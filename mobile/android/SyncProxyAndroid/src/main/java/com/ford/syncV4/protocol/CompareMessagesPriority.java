package com.ford.syncV4.protocol;

import java.util.Comparator;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 18.04.14
 * Time: 12:45
 */
public class CompareMessagesPriority<T extends RunnableWithPriority> implements Comparator<T> {

    @Override
    public int compare(T o1, T o2) {
        int result = 0;

        if (o1.getPriority() == o2.getPriority()) {
            //Logger.d("TRACE: " + o1.getCorrelationId() + " " + o2.getCorrelationId());
            if (o1.getCorrelationId() < o2.getCorrelationId()) {
               result = -1;
            } else {
                result = 1;
            }
        } else if (o1.getPriority() < o2.getPriority()) {
            result = -1;
        } else {
            result = 1;
        }

        return result;
    }
}