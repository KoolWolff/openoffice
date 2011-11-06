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

import com.sun.star.accessibility.*;
import com.sun.star.lang.EventObject;
import com.sun.star.uno.*;
import com.sun.star.accessibility.*;

import java.util.LinkedList;

class QueuedListener
    implements XAccessibleEventListener
{
    public QueuedListener (EventListener aListener)
    {
        maListener = aListener;
    }


    public void disposing( final EventObject aEvent)
    {
        XAccessibleContext xContext = (XAccessibleContext)UnoRuntime.queryInterface(
            XAccessibleContext.class, aEvent.Source);
        if (xContext == null)
        {
            XAccessible xAccessible = (XAccessible)UnoRuntime.queryInterface(
                XAccessible.class, aEvent.Source);
            if (xAccessible != null)
                xContext = xAccessible.getAccessibleContext();
        }
        final XAccessibleContext xSource = xContext;
        EventQueue.Instance().addDisposingEvent (new Runnable() 
            {
                public void run() 
                {
                    if (QueuedListener.this.maListener != null)
                        QueuedListener.this.maListener.disposing (xSource);
                }
            } 
            );
    }

    public void notifyEvent( final AccessibleEventObject aEvent )
    {
        EventQueue.Instance().addEvent (new Runnable()
            {
                public void run()
                {
                    QueuedListener.this.maListener.notifyEvent( aEvent );
                }
            } 
            );
    }

    private EventListener maListener;
}


