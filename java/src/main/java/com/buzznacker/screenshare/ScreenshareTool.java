package com.buzznacker.screenshare;

import com.sun.jna.Native;
import com.sun.jna.platform.win32.*;
import com.sun.jna.platform.win32.Tlhelp32.PROCESSENTRY32;
import com.sun.jna.platform.win32.WinDef.DWORD;
import com.sun.jna.platform.win32.WinDef.HWND;
import com.sun.jna.platform.win32.WinNT.HANDLE;
import com.sun.jna.ptr.IntByReference;

import javax.swing.*;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

public class ScreenshareTool {

    private int selectedPid = 0;

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

    public void setpId(int pId) {
        this.selectedPid = pId;
        setSelectedPid(pId);
    }

    public void stringSearch(String[] args) {
        if(selectedPid == 0)
            System.out.println("There must be a selected PID in order to be able to search a string");
        else
            searchString(args);
    }

    public int getProcIdByWindowName(String name) {
        List<HWND> wHandles = getProcsByWindowNames(name);
        if(wHandles.size() != 1)
            return -1;

        HWND wHandle = wHandles.get(0);
        IntByReference intRef = new IntByReference();
        User32.INSTANCE.GetWindowThreadProcessId(wHandle, intRef);

        return intRef.getValue();
    }

    private List<HWND> getProcsByWindowNames(String name) {
        final User32 USER32 = User32.INSTANCE;

        List<HWND> windows = new ArrayList<>();

        USER32.EnumWindows((hwnd, pointer) -> {
            char[] windowText = new char[512];
            USER32.GetWindowText(hwnd, windowText, 512);
            String wText = Native.toString(windowText);
            if(wText.equals(name))
                windows.add(hwnd);
            return true;
        }, null);

        return windows;
    }

    public Set<Integer> findProcessIdByName(String name) {
        Kernel32 kernel32 = Kernel32.INSTANCE;

        PROCESSENTRY32.ByReference processEntry = new PROCESSENTRY32.ByReference();

        HANDLE snapShot = kernel32.CreateToolhelp32Snapshot(Tlhelp32.TH32CS_SNAPPROCESS, new DWORD(0));

        Set<Integer> pids = new HashSet<>();

        while (kernel32.Process32Next(snapShot, processEntry)) {
            if(Native.toString(processEntry.szExeFile).equals(name))
                pids.add(processEntry.th32ProcessID.intValue());
        }

        kernel32.CloseHandle(snapShot);
        return pids;
    }

    public int getSelectedPid() {
        return selectedPid;
    }

    private native void searchString(String[] strings);

    private native void setSelectedPid(int pId);
}
