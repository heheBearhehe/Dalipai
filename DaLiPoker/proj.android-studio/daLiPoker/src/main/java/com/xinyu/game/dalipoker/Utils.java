package com.xinyu.game.dalipoker;

import android.content.Context;
import android.content.Intent;
import android.net.Uri;

/**
 * Created by lugan on 30/07/2017.
 */

public class Utils {

    private static Context sContext;

    public static void intialize(Context context) {
        sContext = context;
    }

    public static void openUrl(String url) {
        Intent intent = new Intent();
        intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        intent.setAction("android.intent.action.VIEW");
        intent.setData(Uri.parse(url));
        sContext.startActivity(intent);
    }

    public static void sendEmail(String address, String subject) {
        Intent data = new Intent(Intent.ACTION_SENDTO);
        data.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        data.setData(Uri.parse("mailto:" + address));
        data.putExtra(Intent.EXTRA_SUBJECT, subject);
        sContext.startActivity(data);
    }

}
