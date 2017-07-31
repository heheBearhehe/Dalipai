package com.xinyu.game.dalipoker;

/**
 * Created by lugan on 30/07/2017.
 */

public class Application extends android.app.Application {

    @Override
    public void onCreate() {
        super.onCreate();
        Utils.intialize(this);
    }
}
