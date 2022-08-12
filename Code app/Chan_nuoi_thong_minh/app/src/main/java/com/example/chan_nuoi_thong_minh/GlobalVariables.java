package com.example.chan_nuoi_thong_minh;

import java.util.ArrayList;

public class GlobalVariables {
    //public static String[] arraylist = new String[1000];
    //public static ArrayList<String> stringArrayList = new ArrayList<>();
    private String nhietdo;
    private String doam;

    public GlobalVariables(String nhietdo, String doam) {
        this.nhietdo = nhietdo;
        this.doam = doam;
    }

    public String getNhietdo() {
        return nhietdo;
    }

    public void setNhietdo(String nhietdo) {
        this.nhietdo = nhietdo;
    }

    public String getDoam() {
        return doam;
    }

    public void setDoam(String doam) {
        this.doam = doam;
    }
}
