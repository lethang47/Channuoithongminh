package com.example.chan_nuoi_thong_minh;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.Toast;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonArrayRequest;
import com.android.volley.toolbox.Volley;
import com.github.mikephil.charting.charts.LineChart;
import com.github.mikephil.charting.data.Entry;
import com.github.mikephil.charting.data.LineData;
import com.github.mikephil.charting.data.LineDataSet;
import com.github.mikephil.charting.interfaces.datasets.ILineDataSet;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.concurrent.Delayed;

public class Bieudo_temhum extends AppCompatActivity {
    private LineChart bieudo_tem;
    private LineChart bieudo_hum;
    //GlobalVariables variables = new GlobalVariables();
    private ArrayList<GlobalVariables> globalVariablesArrayList;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_bieudo_temhum);

        bieudo_tem = findViewById(R.id.id_bieudo_tem);
        bieudo_hum = findViewById(R.id.id_bieudo_hum);
        getdata_tem();
        getdata_hum();
    }

private void getdata_tem(){
        globalVariablesArrayList = new ArrayList<>();
        String url = "http://192.168.0.23/app_channuoithongminh/getdata_tem_hum.php";
        RequestQueue requestQueue = Volley.newRequestQueue(this);
        JsonArrayRequest jsonArrayRequest = new JsonArrayRequest(Request.Method.GET, url, null,
                new Response.Listener<JSONArray>() {
                    @Override
                    public void onResponse(JSONArray response) {
                        ArrayList<Entry> data = new ArrayList<Entry>();
                        int check = 0;
                        int index_data = 0;
                        float nhietdo_float_tong = 0;
                        for (int i = 0; i < response.length(); i++){
                            try {
                                JSONObject jsonObject = response.getJSONObject(i);
                                String nhietdo = jsonObject.getString("Tem");
                                float nhietdo_float = Float.parseFloat(nhietdo);
                                nhietdo_float_tong = nhietdo_float_tong + nhietdo_float;
                                check = check + 1;
                                if (check == 30) {
                                    check = 0;
                                    float nhietdo_float_tong_trungbinh = nhietdo_float_tong/30;
                                    data.add(new Entry(index_data, nhietdo_float_tong_trungbinh));
                                    index_data = index_data + 1;
                                    nhietdo_float_tong = 0;
                                }

                            } catch (JSONException e) {
                                e.printStackTrace();
                            }
                        }
                        LineDataSet lineDataSet = new LineDataSet(data, "Biểu đồ nhiệt độ");
                        ArrayList<ILineDataSet> dataSets = new ArrayList<>();
                        dataSets.add(lineDataSet);

                        LineData lineData = new LineData(dataSets);
                        bieudo_tem.setData(lineData);
                        bieudo_tem.invalidate();
                    }
                }, new Response.ErrorListener() {
            @Override
            public void onErrorResponse(VolleyError error) {

            }
        });
        requestQueue.add(jsonArrayRequest);
        Toast.makeText(this,String.valueOf(globalVariablesArrayList.size()), Toast.LENGTH_SHORT).show();
    }
    private void getdata_hum(){
        globalVariablesArrayList = new ArrayList<>();
        String url = "http://192.168.0.23/app_channuoithongminh/getdata_tem_hum.php";
        RequestQueue requestQueue = Volley.newRequestQueue(this);
        JsonArrayRequest jsonArrayRequest = new JsonArrayRequest(Request.Method.GET, url, null,
                new Response.Listener<JSONArray>() {
                    @Override
                    public void onResponse(JSONArray response) {
                        ArrayList<Entry> data = new ArrayList<Entry>();
                        int check = 0;
                        int index_data = 0;
                        float doam_float_tong = 0;
                        for (int i = 0; i < response.length(); i++){
                            try {
                                JSONObject jsonObject = response.getJSONObject(i);
                                String doam = jsonObject.getString("Hum");
                                float nhietdo_float = Float.parseFloat(doam);
                                doam_float_tong = doam_float_tong + nhietdo_float;
                                check = check + 1;
                                if (check == 30) {
                                    check = 0;
                                    float nhietdo_float_tong_trungbinh = doam_float_tong/30;
                                    data.add(new Entry(index_data, nhietdo_float_tong_trungbinh));
                                    index_data = index_data + 1;
                                    doam_float_tong = 0;
                                }

                            } catch (JSONException e) {
                                e.printStackTrace();
                            }
                        }
                        LineDataSet lineDataSet = new LineDataSet(data, "Biểu đồ độ ẩm");
                        ArrayList<ILineDataSet> dataSets = new ArrayList<>();
                        dataSets.add(lineDataSet);

                        LineData lineData = new LineData(dataSets);
                        bieudo_hum.setData(lineData);
                        bieudo_hum.invalidate();
                    }
                }, new Response.ErrorListener() {
            @Override
            public void onErrorResponse(VolleyError error) {

            }
        });
        requestQueue.add(jsonArrayRequest);
        Toast.makeText(this,String.valueOf(globalVariablesArrayList.size()), Toast.LENGTH_SHORT).show();
    }
}
