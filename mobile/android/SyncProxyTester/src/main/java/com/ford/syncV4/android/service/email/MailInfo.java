package com.ford.syncV4.android.service.email;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 04.04.14
 * Time: 9:48
 */
public class MailInfo {

    private String to;
    private String subj;
    private String mailBody;

    public MailInfo(String to, String subj, String mailBody) {
        if (to == null) {
            throw new NullPointerException("to");
        }
        if (to.isEmpty()) {
            throw new NullPointerException("to");
        }
        this.to = to;
        this.subj = subj;
        this.mailBody = mailBody;
    }

    public String getTo() {
        return to;
    }

    public String getSubj() {
        return subj;
    }

    public String getMailBody() {
        return mailBody;
    }
}