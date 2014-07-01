
package com.techbooster.sensor;

import android.os.Parcel;
import android.os.Parcelable;

/**
 * Created by tomo on 2014/05/08.
 */
public class SensorInfo implements Parcelable {
    private int mFifoMaxEventCount;
    private int mFifoReservedEventCount;
    private float mMaximumRange;
    private int mMinDelay;
    private String mName;
    private float mPower;
    private float mResolution;
    private int mType;
    private String mVendor;
    private int mVersion;
    private String mToString;

    private String mActivityName;

    public int getFifoMaxEventCount() {
        return mFifoMaxEventCount;
    }

    public void setFifoMaxEventCount(int fifoMaxEventCount) {
        this.mFifoMaxEventCount = fifoMaxEventCount;
    }

    public int getFifoReservedEventCount() {
        return mFifoReservedEventCount;
    }

    public void setFifoReservedEventCount(int fifoReservedEventCount) {
        this.mFifoReservedEventCount = fifoReservedEventCount;
    }

    public SensorInfo() {
    }

    public int describeContents() {
        return 0;
    }

    public void writeToParcel(Parcel out, int flags) {
        out.writeInt(mFifoMaxEventCount);
        out.writeInt(mFifoReservedEventCount);
        out.writeFloat(mMaximumRange);
        out.writeInt(mMinDelay);
        out.writeString(mName);
        out.writeFloat(mPower);
        out.writeFloat(mResolution);
        out.writeInt(mMinDelay);
        out.writeString(mVendor);
        out.writeInt(mVersion);
        out.writeString(mToString);
        out.writeString(mActivityName);
    }

    public static final Parcelable.Creator<SensorInfo> CREATOR = new Parcelable.Creator<SensorInfo>() {
        public SensorInfo createFromParcel(Parcel in) {
            return new SensorInfo(in);
        }

        public SensorInfo[] newArray(int size) {
            return new SensorInfo[size];
        }
    };

    private SensorInfo(Parcel in) {
        mFifoMaxEventCount = in.readInt();
        mFifoReservedEventCount = in.readInt();
        mMaximumRange = in.readFloat();
        mMinDelay = in.readInt();
        mName = in.readString();
        mPower = in.readFloat();
        mResolution = in.readFloat();
        mType = in.readInt();
        mVendor = in.readString();
        mVersion = in.readInt();
        mToString = in.readString();
        mActivityName = in.readString();
    }

    public float getMaximumRange() {
        return mMaximumRange;
    }

    public void setMaximumRange(float maximumRange) {
        this.mMaximumRange = maximumRange;
    }

    public int getMinDelay() {
        return mMinDelay;
    }

    public void setMinDelay(int minDelay) {
        this.mMinDelay = minDelay;
    }

    public String getName() {
        return mName;
    }

    public void setName(String name) {
        this.mName = name;
    }

    public float getPower() {
        return mPower;
    }

    public void setPower(float power) {
        this.mPower = power;
    }

    public float getResolution() {
        return mResolution;
    }

    public void setResolution(float resolution) {
        this.mResolution = resolution;
    }

    public int getType() {
        return mType;
    }

    public void setType(int type) {
        this.mType = type;
    }

    public String getVendor() {
        return mVendor;
    }

    public void setVendor(String vendor) {
        this.mVendor = vendor;
    }

    public int getVersion() {
        return mVersion;
    }

    public void setVersion(int version) {
        this.mVersion = version;
    }

    public String getToString() {
        return mToString;
    }

    public void setToString(String toString) {
        this.mToString = toString;
    }

    public String getActivityName() {
        return mActivityName;
    }

    public void setActivityName(String activityName) {
        mActivityName = activityName;
    }
}
