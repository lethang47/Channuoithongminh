package com.example.chan_nuoi_thong_minh;

public class contentngaygiochoan {
    private String ngaygio;
    private String soluong;

    public contentngaygiochoan(String ngaygio, String soluong) {
        this.ngaygio = ngaygio;
        this.soluong = soluong;
    }

    public String getNgaygio() {
        return ngaygio;
    }

    public void setNgaygio(String ngaygio) {
        this.ngaygio = ngaygio;
    }

    public String getSoluong() {
        return soluong;
    }

    public void setSoluong(String soluong) {
        this.soluong = soluong;
    }
}
