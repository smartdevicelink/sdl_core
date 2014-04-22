package com.ford.syncV4.protocol;

import com.ford.syncV4.util.logger.Logger;

import java.util.Comparator;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 18.04.14
 * Time: 12:45
 */
public class CompareMessagesPriority<T extends RunnableWithPriority> implements Comparator<T> {

    @SuppressWarnings("unused")
    private static final String LOG_TAG = CompareMessagesPriority.class.getSimpleName();

    @Override
    public int compare(T object1, T object2) {
        int result = 0;

        Logger.d(LOG_TAG + " TRACE obj1 priority:" + object1.getPriority() + " " + object1.getCorrelationId());
        Logger.d(LOG_TAG + " TRACE obj2 priority:" + object1.getPriority() + " " + object2.getCorrelationId());

        if (object1.getPriority() == object2.getPriority()) {
            if (object1.getCorrelationId() != -1 && object2.getCorrelationId() != -1) {
                if (object1.getCorrelationId() < object2.getCorrelationId()) {
                    result = -1;
                } else {
                    result = 1;
                }
            }

        } else if (object1.getPriority() < object2.getPriority()) {
            result = -1;
        } else {
            result = 1;
        }

        return result;
    }
}