package com.example.chan_nuoi_thong_minh;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import android.app.AlertDialog;
import android.app.DatePickerDialog;
import android.app.TimePickerDialog;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.DatePicker;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TimePicker;
import android.widget.Toast;

import com.android.volley.AuthFailureError;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonArrayRequest;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.HashMap;
import java.util.Map;

public class Hengio extends AppCompatActivity {
    private DatePickerDialog datePickerDialog;
    private TimePickerDialog timePickerDialog;
    private Button ngay, gio, xacnhan;
    private EditText soluong;
    private ListView danhsachhengio;
    private ArrayList<contenthengiochoan> contenthengiochoanArrayList;
    private contenthengiochoanApdater contenthengiochoanapdater;
    private String host = "192.168.0.23";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_hengio);
        ngay = findViewById(R.id.id_bt_ngay);
        gio = findViewById(R.id.id_bt_gio);
        xacnhan = findViewById(R.id.id_bt_xacnhan);
        soluong = findViewById(R.id.id_et_soluong);

        setTodayDate();
        ngay.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                initDatePicker();
            }
        });

        setTodayTime();
        gio.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                initTimePicker();
            }
        });

        hienthidanhsachhengio();
        xacnhan.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                String url = "http://" + host + "/app_channuoithongminh/insert_hengio.php";
                if (soluong.getText().toString().isEmpty()){
                    Toast.makeText(Hengio.this, "Vui lòng nhập số lượng", Toast.LENGTH_SHORT).show();
                } else {
                    insert_hengio(url);

                }
            }
        });

        //gettime_check();
    }
    private void initDatePicker() {
        DatePickerDialog.OnDateSetListener dateSetlistener = new DatePickerDialog.OnDateSetListener() {
            @Override
            public void onDateSet(DatePicker datePicker, int year, int month, int day) {
                month = month + 1;
                //String date = day + "-" + month + "-" + year;
                if (month < 10) {
                    String monthnew = "0" + month;
                    if (day < 10) {
                        String daynew = "0" + day;
                        ngay.setText(daynew + "-" + monthnew + "-" + year);
                    } else ngay.setText(day + "-" + monthnew + "-" + year);
                } else {
                    if (day < 10) {
                        String daynew = "0" + day;
                        ngay.setText(daynew + "-" + month + "-" + year);
                    } else ngay.setText(day + "-" + month + "-" + year);
                }
            }
        };
        Calendar cal = Calendar.getInstance();
        int year = cal.get(Calendar.YEAR);
        int month = cal.get(Calendar.MONTH);
        int day = cal.get(Calendar.DAY_OF_MONTH);

        int style = AlertDialog.THEME_HOLO_LIGHT;
        datePickerDialog = new DatePickerDialog(this, style, dateSetlistener, year, month, day);
        datePickerDialog.show();
    }
    private void setTodayDate(){
        Calendar cal = Calendar.getInstance();
        int year = cal.get(Calendar.YEAR);
        int month = cal.get(Calendar.MONTH);
        int day = cal.get(Calendar.DAY_OF_MONTH);
        month = month + 1;
        if (month < 10) {
            String monthnew = "0" + month;
            if (day < 10) {
                String daynew = "0" + day;
                ngay.setText(daynew + "-" + monthnew + "-" + year);
            } else ngay.setText(day + "-" + monthnew + "-" + year);
        } else {
            if (day < 10) {
                String daynew = "0" + day;
                ngay.setText(daynew + "-" + month + "-" + year);
            } else ngay.setText(day + "-" + month + "-" + year);
        }
    }
    private void initTimePicker(){
        Calendar cal = Calendar.getInstance();
        int hour = cal.get(Calendar.HOUR_OF_DAY);
        int minute = cal.get(Calendar.MINUTE);
        timePickerDialog = new TimePickerDialog(this, AlertDialog.THEME_HOLO_LIGHT, new TimePickerDialog.OnTimeSetListener() {
            @Override
            public void onTimeSet(TimePicker timePicker, int hour_, int minute_) {

                if (hour_ < 10) {
                    String hournew = "0" + hour_;
                    if (minute_ < 10) {
                        String minutenew = "0" + minute_;
                        gio.setText(hournew + ":" + minutenew);
                    } else gio.setText(hournew + ":" + minute_);
                } else {
                    if (minute_ < 10) {
                        String minutenew = "0" + minute_;
                        gio.setText(hour_ + ":" + minutenew);
                    } else gio.setText(hour_ + ":" + minute_);
                }

            }
        }, hour, minute, true);
        timePickerDialog.show();
    }
    private void setTodayTime(){
        Calendar cal = Calendar.getInstance();
        int hour = cal.get(Calendar.HOUR_OF_DAY);
        int minute = cal.get(Calendar.MINUTE);
        if (hour < 10) {
            String hournew = "0" + hour;
            if (minute < 10) {
                String minutenew = "0" + minute;
                gio.setText(hournew + ":" + minutenew);
            } else gio.setText(hournew + ":" + minute);
        } else {
            if (minute< 10) {
                String minutenew = "0" + minute;
                gio.setText(hour + ":" + minutenew);
            } else gio.setText(hour + ":" + minute);
        }

    }
    private void insert_hengio(String url){
        String ngayhengio = ngay.getText().toString();
        String giohengio = gio.getText().toString();
        String date = ngayhengio + " " + giohengio;
        String soluongchoan = soluong.getText().toString();
        RequestQueue requestQueue = Volley.newRequestQueue(this);
        StringRequest stringRequest = new StringRequest(Request.Method.POST, url,
                new Response.Listener<String>() {
                    @Override
                    public void onResponse(String response) {
                        if (response.equals("success")){
                            Toast.makeText(Hengio.this, "Hẹn giờ thành công", Toast.LENGTH_SHORT).show();
                            soluong.setText("");
                            hienthidanhsachhengio();
                        }
                    }
                }, new Response.ErrorListener() {
            @Override
            public void onErrorResponse(VolleyError error) {

            }
        }){
            @Nullable
            @Override
            protected Map<String, String> getParams() throws AuthFailureError {
                Map<String, String> params = new HashMap<>();
                params.put("time", date);
                params.put("soluong", soluongchoan);
                return params;
            }
        };
        requestQueue.add(stringRequest);
    }
    private void hienthidanhsachhengio(){
        String url = "http://" + host + "/app_channuoithongminh/getdata_hengio.php";
        danhsachhengio = findViewById(R.id.id_lv_danhsachhengio);
        contenthengiochoanArrayList = new ArrayList<>();
        RequestQueue requestQueue = Volley.newRequestQueue(this);
        JsonArrayRequest jsonArrayRequest = new JsonArrayRequest(Request.Method.GET, url, null,
                new Response.Listener<JSONArray>() {
                    @Override
                    public void onResponse(JSONArray response) {
                        for (int i = response.length() - 1; i >= 0; i--){
                            try {
                                JSONObject jsonObject = response.getJSONObject(i);
                                String date = jsonObject.getString("Time");
                                String soluong = jsonObject.getString("Soluong");
                                contenthengiochoanArrayList.add(new contenthengiochoan("Ngày giờ hẹn: " + date, "Số lượng: " + soluong + " gam"));
                            } catch (JSONException e) {
                                e.printStackTrace();
                            }
                        }
                        contenthengiochoanapdater.notifyDataSetChanged();
                    }
                }, new Response.ErrorListener() {
            @Override
            public void onErrorResponse(VolleyError error) {

            }
        });
        requestQueue.add(jsonArrayRequest);

        contenthengiochoanapdater = new contenthengiochoanApdater(Hengio.this, R.layout.content_hengiochoan, contenthengiochoanArrayList);
        danhsachhengio.setAdapter(contenthengiochoanapdater);
    }

}