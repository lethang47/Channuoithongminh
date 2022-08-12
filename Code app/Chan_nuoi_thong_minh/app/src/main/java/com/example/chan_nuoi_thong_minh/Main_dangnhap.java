package com.example.chan_nuoi_thong_minh;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.RecyclerView;
import androidx.swiperefreshlayout.widget.SwipeRefreshLayout;

import android.content.Intent;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.os.Handler;
import android.util.Log;
import android.view.View;
import android.widget.Adapter;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;

import com.android.volley.AuthFailureError;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonArrayRequest;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;
import com.github.mikephil.charting.utils.Utils;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

public class Main_dangnhap extends AppCompatActivity {
    private Switch quat, dieuhoa, maysuoi, den, phunsuong;
    private TextView temp, hum;
    private EditText thietlap_nhietdo, thietlap_doam, soluongthucan;
    private Button thietlap, xoa_thietlap, xembieudo_temhum, choan, hengio;
    private ListView danhsachchoan;
    private ArrayList<contentngaygiochoan> contentngaygiochoanArrayList;
    private contentngaygiochoanApdater contentngaygiochoanapdater;
    private String host = "192.168.0.23";
    final Handler handler1 = new Handler();
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main_dangnhap);
        Anhxa(); // Ánh xạ đến cái id

        set_switch(); // thiết lập trạng thái switch khi mới vào app
        chose_switch(); // Bật tắt các switch

        set_tem_hum(); // Thiết lập nhiệt độ, độ ẩm
        xem_bieudo_temhum(); // Xem biểu đồ nhiệt độ, độ ẩm

        refresh_data(); // refresh lại data khi có cập nhật

        set_thietlap(); // set lại giá trị độ ẩm thiết lập và nhiệt độ thiết lập khi vào app
        thietlap(); // thiết lập nhiệt độ độ ẩm tự động

        //Button thực hiện cho ăn
        choan.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (soluongthucan.getText().toString().isEmpty()) Toast.makeText(Main_dangnhap.this, "Vui lòng nhập số lượng thức ăn", Toast.LENGTH_SHORT).show();
                else choan();  // Thực hiện cho ăn
            }
        });

        hienthidanhsachchoan(); // Hiển thị ra danh sách đã cho ăn

        //Button hẹn giờ

        hengio.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(Main_dangnhap.this, Hengio.class);
                startActivity(intent);
            }
        });

        get_choan_xoa();
    }
    private void Anhxa(){
        quat = findViewById(R.id.id_switch_quat);
        dieuhoa = findViewById(R.id.id_switch_dieuhoa);
        maysuoi = findViewById(R.id.id_switch_maysuoi);
        den = findViewById(R.id.id_switch_den);
        phunsuong = findViewById(R.id.id_switch_phunsuong);
        temp = findViewById(R.id.id_tv_nhietdo);
        hum = findViewById(R.id.id_tv_doam);
        thietlap_nhietdo = findViewById(R.id.id_et_thietlap_nhietdo);
        thietlap_doam = findViewById(R.id.id_et_thietlap_doam);
        thietlap = findViewById(R.id.id_bt_thietlap);
        xoa_thietlap = findViewById(R.id.id_bt_xoathietlap);
        xembieudo_temhum = findViewById(R.id.id_bt_xembieudo_temhum);
        soluongthucan = findViewById(R.id.id_et_soluongthucan);
        choan = findViewById(R.id.id_bt_choan);
        hengio = findViewById(R.id.id_bt_hengio);
    }

    private void chose_switch(){
        String url = "http://" + host + "/app_channuoithongminh/insert_switch_data.php";
        quat.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                if (b){
                    Toast.makeText(Main_dangnhap.this, "Quạt đã được bật", Toast.LENGTH_SHORT).show();
                    insert_switch_data(url, "1", "1");
                } else {
                    Toast.makeText(Main_dangnhap.this, "Quạt đã được tắt", Toast.LENGTH_SHORT).show();
                    insert_switch_data(url, "0", "1");
                }
            }
        });

        dieuhoa.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                if (b){
                    Toast.makeText(Main_dangnhap.this, "Điều hòa đã được bật", Toast.LENGTH_SHORT).show();
                    insert_switch_data(url, "1", "2");
                } else {
                    Toast.makeText(Main_dangnhap.this, "Điều hòa đã được tắt", Toast.LENGTH_SHORT).show();
                    insert_switch_data(url, "0", "2");
                }
            }
        });

        maysuoi.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                if (b){
                    Toast.makeText(Main_dangnhap.this, "Máy sưởi đã được bật", Toast.LENGTH_SHORT).show();
                    insert_switch_data(url, "1", "3");
                } else {
                    Toast.makeText(Main_dangnhap.this, "Máy sưởi đã được tắt", Toast.LENGTH_SHORT).show();
                    insert_switch_data(url, "0", "3");
                }
            }
        });

        den.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                if (b){
                    Toast.makeText(Main_dangnhap.this, "Đèn đã được bật", Toast.LENGTH_SHORT).show();
                    insert_switch_data(url, "1", "4");
                } else {
                    Toast.makeText(Main_dangnhap.this, "Đèn đã được tắt", Toast.LENGTH_SHORT).show();
                    insert_switch_data(url, "0", "4");
                }
            }
        });

        phunsuong.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                if (b){
                    Toast.makeText(Main_dangnhap.this, "Phun sương đã được bật", Toast.LENGTH_SHORT).show();
                    insert_switch_data(url, "1", "5");
                } else {
                    Toast.makeText(Main_dangnhap.this, "Phun sương đã được tắt", Toast.LENGTH_SHORT).show();
                    insert_switch_data(url, "0", "5");
                }
            }
        });
    }
    private void insert_switch_data(String url, String status, String id){
        RequestQueue requestQueue = Volley.newRequestQueue(Main_dangnhap.this);
        StringRequest stringRequest = new StringRequest(Request.Method.POST, url,
                new Response.Listener<String>() {
                    @Override
                    public void onResponse(String response) {
//                        if (response.trim().equals("success")){
//                            Toast.makeText(Main_dangnhap.this, "OK", Toast.LENGTH_SHORT).show();
//                        } else {
//                            Toast.makeText(Main_dangnhap.this, "NO", Toast.LENGTH_SHORT).show();
//                        }
                    }
                }, new Response.ErrorListener() {
            @Override
            public void onErrorResponse(VolleyError error) {
                Toast.makeText(Main_dangnhap.this, "ERROR", Toast.LENGTH_SHORT).show();
                Log.d("AAA", error.toString());
            }
        }){
            @Nullable
            @Override
            protected Map<String, String> getParams() throws AuthFailureError {
                Map<String, String> params = new HashMap<>();
                params.put("status", status);
                params.put("id", id);
                return params;
            }
        };
        requestQueue.add(stringRequest);
    }
    private void set_switch(){
        String url = "http://" + host + "/app_channuoithongminh/getdata_switch.php";
        RequestQueue requestQueue = Volley.newRequestQueue(this);
        JsonArrayRequest jsonArrayRequest = new JsonArrayRequest(Request.Method.GET, url, null,
                new Response.Listener<JSONArray>() {
                    @Override
                    public void onResponse(JSONArray response) {
                        for(int i = 0; i < response.length(); i++){
                            try {
                                JSONObject jsonObject = response.getJSONObject(i);
                                int id = jsonObject.getInt("Id");
                                int status = jsonObject.getInt("Status");
                                switch(id){
                                    case 1:
                                        if (status == 1) quat.setChecked(true); else quat.setChecked(false);
                                        break;
                                    case 2:
                                        if (status == 1) dieuhoa.setChecked(true); else dieuhoa.setChecked(false);
                                        break;
                                    case 3:
                                        if (status == 1) maysuoi.setChecked(true); else maysuoi.setChecked(false);
                                        break;
                                    case 4:
                                        if (status == 1) den.setChecked(true); else den.setChecked(false);
                                        break;
                                    case 5:
                                        if (status == 1) phunsuong.setChecked(true); else phunsuong.setChecked(false);
                                        break;
                                }
                            } catch (JSONException e) {
                                e.printStackTrace();
                            }

                        }
                    }
                }, new Response.ErrorListener() {
            @Override
            public void onErrorResponse(VolleyError error) {

            }
        });
        requestQueue.add(jsonArrayRequest);
    }

    private void set_tem_hum(){
        String url = "http://" + host + "/app_channuoithongminh/getdata_tem_hum.php";
        RequestQueue requestQueue = Volley.newRequestQueue(this);
        JsonArrayRequest jsonArrayRequest = new JsonArrayRequest(Request.Method.GET, url, null,
                new Response.Listener<JSONArray>() {
                    @Override
                    public void onResponse(JSONArray response) {
                        try {
                            JSONObject jsonObject = response.getJSONObject(response.length() - 1);
                            String nhietdo = jsonObject.getString("Tem");
                            String doam = jsonObject.getString("Hum");
                            temp.setText(nhietdo+"*C");
                            hum.setText(doam+"%");
                        } catch (JSONException e) {
                            e.printStackTrace();
                        }
                    }
                }, new Response.ErrorListener() {
            @Override
            public void onErrorResponse(VolleyError error) {

            }
        });
        requestQueue.add(jsonArrayRequest);
    }
    private void xem_bieudo_temhum(){
        xembieudo_temhum.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(Main_dangnhap.this, Bieudo_temhum.class);
                startActivity(intent);
            }
        });
    }

    private void refresh_data(){
        final Handler handler = new Handler();
        Runnable refresh = new Runnable() {
            @Override
            public void run() {
                // data request
                set_tem_hum();
                set_switch();
                handler.postDelayed(this, 1000);
            }
        };
        handler.postDelayed(refresh, 1000);
    }

    private void thietlap(){
        String url = "http://" + host + "/app_channuoithongminh/insert_thietlap_data.php";
        thietlap.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                    Toast.makeText(Main_dangnhap.this, "Đã thiết lập", Toast.LENGTH_SHORT).show();
                    String nhietdo_thietlap = thietlap_nhietdo.getText().toString();
                    String doam_thietlap = thietlap_doam.getText().toString();
                    if ((nhietdo_thietlap.isEmpty()) || (doam_thietlap.isEmpty())){
                        Toast.makeText(Main_dangnhap.this, "Vui lòng nhập nhiệt độ, độ ẩm", Toast.LENGTH_SHORT).show();
                    } else thietlap_insert_data(url, nhietdo_thietlap, doam_thietlap);
            }
        });
        xoa_thietlap.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                    thietlap_insert_data(url, "0", "0");
                    thietlap_nhietdo.setText("");
                    thietlap_doam.setText("");
            }
        });

    }
    private void thietlap_insert_data(String url, String nhietdo_thietlap, String doam_thietlap){
        RequestQueue requestQueue = Volley.newRequestQueue(this);
        StringRequest stringRequest = new StringRequest(Request.Method.POST, url,
                new Response.Listener<String>() {
                    @Override
                    public void onResponse(String response) {

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
                params.put("Tem",nhietdo_thietlap);
                params.put("Hum",doam_thietlap);
                params.put("Id", "1");
                return params;
            }
        };
        requestQueue.add(stringRequest);
    }
    private void set_thietlap(){
        String url = "http://" + host + "/app_channuoithongminh/getdata_thietlap.php";
        RequestQueue requestQueue = Volley.newRequestQueue(this);
        JsonArrayRequest jsonArrayRequest = new JsonArrayRequest(Request.Method.GET, url, null,
                new Response.Listener<JSONArray>() {
                    @Override
                    public void onResponse(JSONArray response) {
                        try {
                            JSONObject jsonObject = response.getJSONObject(0);
                            String nhietdo_thietlap = jsonObject.getString("Tem");
                            String doam_thietlap = jsonObject.getString("Hum");
                            if ((nhietdo_thietlap.compareTo("0.00")!=0) && (doam_thietlap.compareTo("0.00")!=0)){
                                thietlap_nhietdo.setText(nhietdo_thietlap);
                                thietlap_doam.setText(doam_thietlap);
                            }
                        } catch (JSONException e) {
                            e.printStackTrace();
                        }
                    }
                }, new Response.ErrorListener() {
            @Override
            public void onErrorResponse(VolleyError error) {

            }
        });
        requestQueue.add(jsonArrayRequest);
    }
    private void choan(){
        String url = "http://" + host + "/app_channuoithongminh/insert_choan.php";
        String soluong = soluongthucan.getText().toString();
        RequestQueue requestQueue = Volley.newRequestQueue(this);
        StringRequest stringRequest = new StringRequest(Request.Method.POST, url,
                new Response.Listener<String>() {
                    @Override
                    public void onResponse(String response) {
                        if(response.equals("success")){
                            Toast.makeText(Main_dangnhap.this, "Đang cho ăn", Toast.LENGTH_SHORT).show();
                            soluongthucan.setText("");
                            hienthidanhsachchoan();
                            new CountDownTimer(5000, 1000){

                                @Override
                                public void onTick(long l) {

                                }

                                @Override
                                public void onFinish() {
                                    hienthithongbaochoanthanhcong();
                                }
                            }.start();


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
                params.put("soluong", soluong);
                params.put("check", "0");
                return params;
            }
        };
        requestQueue.add(stringRequest);
    }
    private void hienthidanhsachchoan(){
        danhsachchoan = findViewById(R.id.id_lv_danhsachthucan);
        contentngaygiochoanArrayList = new ArrayList<>();
        String url = "http://" + host + "/app_channuoithongminh/getdata_choan.php";
        RequestQueue requestQueue = Volley.newRequestQueue(this);
        JsonArrayRequest jsonArrayRequest = new JsonArrayRequest(Request.Method.GET, url, null,
                new Response.Listener<JSONArray>() {
                    @Override
                    public void onResponse(JSONArray response) {
                        int check = 0;
                        try {
                            JSONObject jsonObject1 = response.getJSONObject(response.length() - 1);

                        } catch (JSONException e) {
                            e.printStackTrace();
                        }

                        for (int i = response.length() - 1; i >= 0; i--){
                            try {
                                JSONObject jsonObject = response.getJSONObject(i);
                                String soluong = jsonObject.getString("Soluong");
                                String time = jsonObject.getString("Time");
                                String[] time1 = time.split(":");
                                String timenew = time1[0] + ":" + time1[1];
                                contentngaygiochoanArrayList.add(new contentngaygiochoan("Ngày giờ cho ăn: " + timenew, "Số lượng: " + soluong + " gam"));
                                check = check + 1;
                                if (check == 5) break;
                            } catch (JSONException e) {
                                e.printStackTrace();
                            }
                        }
                        contentngaygiochoanapdater.notifyDataSetChanged();
                    }
                }, new Response.ErrorListener() {
            @Override
            public void onErrorResponse(VolleyError error) {

            }
        });
        requestQueue.add(jsonArrayRequest);

        contentngaygiochoanapdater = new contentngaygiochoanApdater(Main_dangnhap.this, R.layout.content_ngaygiochoan, contentngaygiochoanArrayList);
        danhsachchoan.setAdapter(contentngaygiochoanapdater);
    }
    private void hienthithongbaochoanthanhcong(){
        String url = "http://" + host + "/app_channuoithongminh/getdata_choan.php";
        RequestQueue requestQueue = Volley.newRequestQueue(this);
        JsonArrayRequest jsonArrayRequest = new JsonArrayRequest(Request.Method.GET, url, null,
                new Response.Listener<JSONArray>() {
                    @Override
                    public void onResponse(JSONArray response) {
                        try {
                            JSONObject jsonObject1 = response.getJSONObject(response.length() - 1);
                            String check = jsonObject1.getString("Check");

                            if (check.compareTo("1") == 0){
                                Toast.makeText(Main_dangnhap.this, "Đã cho ăn", Toast.LENGTH_SHORT).show();
                            } else Toast.makeText(Main_dangnhap.this, "Cho ăn không thành công", Toast.LENGTH_SHORT).show();

                        } catch (JSONException e) {
                            e.printStackTrace();
                        }
                    }
                }, new Response.ErrorListener() {
            @Override
            public void onErrorResponse(VolleyError error) {

            }
        });
        requestQueue.add(jsonArrayRequest);
    }
    private void get_choan_xoa(){
        String url = "http://" + host + "/app_channuoithongminh/getdata_choan.php";
        RequestQueue requestQueue = Volley.newRequestQueue(this);
        JsonArrayRequest jsonArrayRequest = new JsonArrayRequest(Request.Method.GET, url, null,
                new Response.Listener<JSONArray>() {
                    @Override
                    public void onResponse(JSONArray response) {
                        if (response.length() > 5){
                            for (int i = 0; i < response.length() - 5; i++){
                                try {
                                    JSONObject jsonObject = response.getJSONObject(i);
                                    String id = jsonObject.getString("Id");
                                    xoahchoan(id);
                                } catch (JSONException e) {
                                    e.printStackTrace();
                                }
                            }
                        }
                    }
                }, new Response.ErrorListener() {
            @Override
            public void onErrorResponse(VolleyError error) {

            }
        });
        requestQueue.add(jsonArrayRequest);
    }
    private void xoahchoan(String id){
        String url = "http://" + host + "/app_channuoithongminh/xoa_choan.php";
        String soluong = soluongthucan.getText().toString();
        RequestQueue requestQueue = Volley.newRequestQueue(this);
        StringRequest stringRequest = new StringRequest(Request.Method.POST, url,
                new Response.Listener<String>() {
                    @Override
                    public void onResponse(String response) {
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
                params.put("id", id);
                return params;
            }
        };
        requestQueue.add(stringRequest);
    }
//    @Override
//    public void onRefresh() {
//        set_tem_hum();
//        Handler handler = new Handler();
//        handler.postDelayed(new Runnable() {
//            @Override
//            public void run() {
//                swipeRefreshLayout.setRefreshing(false);
//            }
//        }, 1000);
//    }
}