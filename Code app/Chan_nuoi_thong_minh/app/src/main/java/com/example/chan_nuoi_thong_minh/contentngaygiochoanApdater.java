package com.example.chan_nuoi_thong_minh;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import java.util.List;

public class contentngaygiochoanApdater extends BaseAdapter {
    private Context context;
    private int layout;
    private List<contentngaygiochoan> contentngaygiochoanList;

    public contentngaygiochoanApdater(Context context, int layout, List<contentngaygiochoan> contentngaygiochoanList) {
        this.context = context;
        this.layout = layout;
        this.contentngaygiochoanList = contentngaygiochoanList;
    }

    @Override
    public int getCount() {
        return contentngaygiochoanList.size();
    }

    @Override
    public Object getItem(int i) {
        return null;
    }

    @Override
    public long getItemId(int i) {
        return 0;
    }

    @Override
    public View getView(int i, View view, ViewGroup viewGroup) {
        LayoutInflater inflater = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        view = inflater.inflate(layout, null);
        TextView ngaygiochoan = view.findViewById(R.id.id_ngaygio);
        TextView soluongchoan = view.findViewById(R.id.id_soluong);
        contentngaygiochoan contentngaygiochoAn = contentngaygiochoanList.get(i);
        ngaygiochoan.setText(contentngaygiochoAn.getNgaygio());
        soluongchoan.setText(contentngaygiochoAn.getSoluong());
        return view;
    }
}
