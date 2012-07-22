/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/

package testcase.uno.sw;

import static org.junit.Assert.*;

import java.math.BigInteger;

import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;
import org.openoffice.test.common.FileUtil;
import org.openoffice.test.common.Testspace;
import org.openoffice.test.uno.UnoApp;

import com.sun.star.beans.PropertyValue;
import com.sun.star.container.XNameAccess;
import com.sun.star.container.XNamed;
import com.sun.star.frame.XStorable;
import com.sun.star.lang.XMultiServiceFactory;
import com.sun.star.text.ControlCharacter;
import com.sun.star.text.XBookmarksSupplier;
import com.sun.star.text.XText;
import com.sun.star.text.XTextContent;
import com.sun.star.text.XTextCursor;
import com.sun.star.text.XTextDocument;
import com.sun.star.text.XTextRange;
import com.sun.star.uno.UnoRuntime;

class BookmarkHashes {
	public BigInteger m_nSetupHash;
	public BigInteger m_nInsertRandomHash;
	public BigInteger m_nDeleteRandomHash;
	public BigInteger m_nLinebreakHash;
	public BigInteger m_nOdfReloadHash;
	public BigInteger m_nMsWordReloadHash;

	public void assertExpectation(BookmarkHashes aExpectation) {
		assertEquals(aExpectation.m_nSetupHash, m_nSetupHash);
		assertEquals(aExpectation.m_nInsertRandomHash, m_nInsertRandomHash);
		assertEquals(aExpectation.m_nDeleteRandomHash, m_nDeleteRandomHash);
		assertEquals(aExpectation.m_nLinebreakHash, m_nLinebreakHash);
		assertEquals(aExpectation.m_nOdfReloadHash, m_nOdfReloadHash);
		assertEquals(aExpectation.m_nMsWordReloadHash, m_nMsWordReloadHash);
	}

	static public java.math.BigInteger getBookmarksHash(XTextDocument xDoc) throws Exception {
		StringBuffer buffer = new StringBuffer("");
		XBookmarksSupplier xBookmarksSupplier = (XBookmarksSupplier) UnoRuntime.queryInterface(XBookmarksSupplier.class, xDoc);
		XNameAccess xBookmarks = xBookmarksSupplier.getBookmarks();
		for (String sBookmarkname : xBookmarks.getElementNames()) {
			Object xBookmark = xBookmarks.getByName(sBookmarkname);
			XTextContent xBookmarkAsContent = (XTextContent) UnoRuntime.queryInterface(XTextContent.class, xBookmark);
			buffer.append(sBookmarkname);
			buffer.append(":");
			buffer.append(xBookmarkAsContent.getAnchor().getString());
			buffer.append(";");
		}
		java.security.MessageDigest sha1 = java.security.MessageDigest.getInstance("SHA-1");
		sha1.reset();
		sha1.update(buffer.toString().getBytes());
		return new java.math.BigInteger(sha1.digest());
	}
}

public class CheckBookmarks {
	private XTextDocument m_xDoc = null;
	private XTextDocument m_xOdfReloadedDoc = null;
	private XTextDocument m_xMsWordReloadedDoc = null;
	private final BookmarkHashes actualHashes = new BookmarkHashes();

	private BookmarkHashes getDEV300m41Expectations() {
		BookmarkHashes result = new BookmarkHashes();
		result.m_nSetupHash = new BigInteger("-4b0706744e8452fe1ae9d5e1c28cf70fb6194795", 16);
		result.m_nInsertRandomHash = new BigInteger("25aa0fad3f4881832dcdfe658ec2efa8a1a02bc5", 16);
		result.m_nDeleteRandomHash = new BigInteger("-3ec87e810b46d734677c351ad893bbbf9ea10f55", 16);
		result.m_nLinebreakHash = new BigInteger("3ae08c284ea0d6e738cb43c0a8105e718a633550", 16);
		result.m_nOdfReloadHash = new BigInteger("3ae08c284ea0d6e738cb43c0a8105e718a633550", 16);
		result.m_nMsWordReloadHash = new BigInteger("3ae08c284ea0d6e738cb43c0a8105e718a633550", 16);
		return result;
	}

	@Test
	public void checkBookmarks() throws Exception {
		actualHashes.assertExpectation(getDEV300m41Expectations());
	}

	@Before
	public void setUpDocuments() throws Exception {
		m_xDoc = UnoRuntime.queryInterface(XTextDocument.class, app.newDocument("swriter"));
		setupBookmarks();
		actualHashes.m_nSetupHash = BookmarkHashes.getBookmarksHash(m_xDoc);
		insertRandomParts(200177);
		actualHashes.m_nInsertRandomHash = BookmarkHashes.getBookmarksHash(m_xDoc);
		deleteRandomParts(4711);
		actualHashes.m_nDeleteRandomHash = BookmarkHashes.getBookmarksHash(m_xDoc);
		insertLinebreaks(007);
		actualHashes.m_nLinebreakHash = BookmarkHashes.getBookmarksHash(m_xDoc);
		m_xOdfReloadedDoc = reloadFrom("StarOffice XML (Writer)", "odf");
		actualHashes.m_nOdfReloadHash = BookmarkHashes.getBookmarksHash(m_xOdfReloadedDoc);
		m_xMsWordReloadedDoc = reloadFrom("MS Word 97", "doc");
		actualHashes.m_nMsWordReloadHash = BookmarkHashes.getBookmarksHash(m_xMsWordReloadedDoc);
	}

	@After
	public void tearDownDocuments() {
		app.closeDocument(m_xDoc);
		app.closeDocument(m_xOdfReloadedDoc);
		app.closeDocument(m_xMsWordReloadedDoc);
	}

	@BeforeClass
	public static void setUpConnection() throws Exception {
		app.start();
	}

	@AfterClass
	public static void tearDownConnection() throws Exception {
		app.close();
	}

	private static final UnoApp app = new UnoApp();

	private void setupBookmarks() throws Exception {
		XText xText = m_xDoc.getText();
		for (int nPara = 0; nPara < 10; ++nPara) {
			for (int nBookmark = 0; nBookmark < 100; ++nBookmark) {
				insertBookmark(xText.createTextCursor(), "P" + nPara + "word" + nBookmark, "P" + nPara + "word" + nBookmark);
				XTextCursor xWordCrsr = xText.createTextCursor();
				xWordCrsr.setString(" ");
			}
			XTextCursor xParaCrsr = xText.createTextCursor();
			XTextRange xParaCrsrAsRange = (XTextRange) UnoRuntime.queryInterface(XTextRange.class, xParaCrsr);
			xText.insertControlCharacter(xParaCrsrAsRange, ControlCharacter.PARAGRAPH_BREAK, false);
		}
	}

	private void insertRandomParts(long seed) throws Exception {
		java.util.Random rnd = new java.util.Random(seed);
		XTextCursor xCrsr = m_xDoc.getText().createTextCursor();
		for (int i = 0; i < 600; i++) {
			xCrsr.goRight((short) rnd.nextInt(100), false);
			xCrsr.setString(Long.toString(rnd.nextLong()));
		}
	}

	private void deleteRandomParts(long seed) throws Exception {
		java.util.Random rnd = new java.util.Random(seed);
		XTextCursor xCrsr = m_xDoc.getText().createTextCursor();
		for (int i = 0; i < 600; i++) {
			xCrsr.goRight((short) rnd.nextInt(100), false);
			xCrsr.goRight((short) rnd.nextInt(20), true);
			xCrsr.setString("");
		}
	}

	private void insertLinebreaks(long seed) throws Exception {
		XText xText = m_xDoc.getText();
		java.util.Random rnd = new java.util.Random(seed);
		XTextCursor xCrsr = m_xDoc.getText().createTextCursor();
		for (int i = 0; i < 30; i++) {
			xCrsr.goRight((short) rnd.nextInt(300), false);
			XTextRange xCrsrAsRange = (XTextRange) UnoRuntime.queryInterface(XTextRange.class, xCrsr);
			xText.insertControlCharacter(xCrsrAsRange, ControlCharacter.PARAGRAPH_BREAK, false);
		}
	}

	private void insertBookmark(XTextCursor crsr, String name, String content) throws Exception {
		XMultiServiceFactory xDocFactory = (XMultiServiceFactory) UnoRuntime.queryInterface(XMultiServiceFactory.class, m_xDoc);

		Object xBookmark = xDocFactory.createInstance("com.sun.star.text.Bookmark");
		XTextContent xBookmarkAsTextContent = (XTextContent) UnoRuntime.queryInterface(XTextContent.class, xBookmark);
		crsr.setString(content);
		XNamed xBookmarkAsNamed = (XNamed) UnoRuntime.queryInterface(XNamed.class, xBookmark);
		xBookmarkAsNamed.setName(name);
		m_xDoc.getText().insertTextContent(crsr, xBookmarkAsTextContent, true);
	}

	private XTextDocument reloadFrom(String sFilter, String sExtension) throws Exception {
		String filePath = Testspace.getPath("output/Bookmarktest." + sExtension);
		PropertyValue[] aStoreProperties = new PropertyValue[2];
		aStoreProperties[0] = new PropertyValue();
		aStoreProperties[1] = new PropertyValue();
		aStoreProperties[0].Name = "Override";
		aStoreProperties[0].Value = true;
		aStoreProperties[1].Name = "FilterName";
		aStoreProperties[1].Value = sFilter;
		XStorable xStorable = (XStorable) UnoRuntime.queryInterface(XStorable.class, m_xDoc);
		xStorable.storeToURL(FileUtil.getUrl(filePath), aStoreProperties);

		return UnoRuntime.queryInterface(XTextDocument.class, app.loadDocument(filePath));
	}
}
