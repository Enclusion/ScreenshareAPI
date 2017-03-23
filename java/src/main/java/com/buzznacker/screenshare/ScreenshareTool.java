package com.buzznacker.screenshare;

import javax.swing.*;

public class ScreenshareTool {

    public ScreenshareTool() {
        if(!System.getProperty("sun.arch.data.model").equals("64")) {
            JOptionPane.showMessageDialog(null, "You must be running Java 64 bit in order to run this program", "Error", JOptionPane.ERROR_MESSAGE);
            System.exit(0);
        }
        try {
            System.loadLibrary("SSLibrary");
        } catch (UnsatisfiedLinkError | NullPointerException e) {
            System.err.println("Cannot find the library make sure \"SSLibrary.dll\" is in the same path.");
            System.exit(0);
        }
    }

    /*public static void main(String[] args) {
        ScreenshareTool sstool = new ScreenshareTool();

        int pid = sstool.getProcessByNameAndWindowTitle("javaw.exe", "Minecraft 1.7.10");

        sstool.setSelectedPid(pid);

        sstool.searchString(new String[] {"mememe", "Minecraft"});
    } */

    public native void searchString(String[] strings);

    public native void setSelectedPid(int pId);

    public native String[] getProcessHandles();

    public native int getSelectedPid();

    public native int getProcessByNameAndWindowTitle(String processName, String windowName);

    public native int getProcessIdByName(String processName);
}
