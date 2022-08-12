package com.example.chan_nuoi_thong_minh;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import java.util.List;

public class contenthengiochoanApdater extends BaseAdapter {
    private Context context;
    private int layout;
    private List<contenthengiochoan> contenthengiochoanList;

    public contenthengiochoanApdater(Context context, int layout, List<contenthengiochoan> contenthengiochoanList) {
        this.context = context;
        this.layout = layout;
        this.contenthengiochoanList = contenthengiochoanList;
    }

    @Override
    public int getCount() {
        return contenthengiochoanList.size();
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

        TextView ngaygiohen = view.findViewById(R.id.id_ngaygiohengio);
        TextView soluonghen = view.findViewById(R.id.id_soluonghengio);

        contenthengiochoan contenthengiochoAn = contenthengiochoanList.get(i);
        ngaygiohen.setText(contenthengiochoAn.getNgaygiohen());
        soluonghen.setText(contenthengiochoAn.getSoluonghen());

        return view;
    }
}
