package com.example.chan_nuoi_thong_minh;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonArrayRequest;
import com.android.volley.toolbox.Volley;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

public class MainActivity extends AppCompatActivity {
    private Button dangnhap;
    private EditText taikhoan, matkhau;
    private String host = "192.168.0.23";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        dangnhap = findViewById(R.id.button_dangnhap);
        taikhoan = findViewById(R.id.id_et_taikhoan);
        matkhau = findViewById(R.id.id_et_matkhau);
        String url = "http://" + host + "/app_channuoithongminh/getdata_account.php";
        dangnhap.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                RequestQueue requestQueue = Volley.newRequestQueue(view.getContext());
                JsonArrayRequest jsonArrayRequest = new JsonArrayRequest(Request.Method.GET, url, null,
                        new Response.Listener<JSONArray>() {
                            @Override
                            public void onResponse(JSONArray response) {
                                try {
                                    JSONObject jsonObject = response.getJSONObject(0);
                                    String nd_taikhoan = taikhoan.getText().toString();
                                    String nd_matkhau = matkhau.getText().toString();
                                    String data_taikhoan = jsonObject.getString("Username");
                                    String data_matkhua = jsonObject.getString("Password");
                                    Toast.makeText(MainActivity.this, data_matkhua + data_taikhoan, Toast.LENGTH_SHORT).show();
                                    if ((nd_taikhoan.compareTo(data_taikhoan) == 0) && (nd_matkhau.compareTo(data_matkhua) ==0)){
                                        Toast.makeText(MainActivity.this, "Đăng nhập thành công", Toast.LENGTH_SHORT).show();
                                        Intent intent = new Intent(MainActivity.this, Main_dangnhap.class);
                                        startActivity(intent);
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
        });
    }
}