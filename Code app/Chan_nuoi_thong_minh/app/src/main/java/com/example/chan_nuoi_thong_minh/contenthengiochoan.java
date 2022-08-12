package com.example.chan_nuoi_thong_minh;

public class contenthengiochoan {
    private String ngaygiohen;
    private String soluonghen;

    public contenthengiochoan(String ngaygiohen, String soluonghen) {
        this.ngaygiohen = ngaygiohen;
        this.soluonghen = soluonghen;
    }

    public String getNgaygiohen() {
        return ngaygiohen;
    }

    public void setNgaygiohen(String ngaygiohen) {
        this.ngaygiohen = ngaygiohen;
    }

    public String getSoluonghen() {
        return soluonghen;
    }

    public void setSoluonghen(String soluonghen) {
        this.soluonghen = soluonghen;
    }
}
