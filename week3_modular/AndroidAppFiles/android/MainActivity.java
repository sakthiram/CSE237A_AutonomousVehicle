package com.example.debjit.myapplication;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;


import java.io.BufferedWriter;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.ArrayList;

public class MainActivity extends Activity {
    private Socket socket;

    private static final int SERVERPORT = 9225;
    private static final String SERVER_IP = "192.168.43.137";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        new Thread(new ClientThread()).start();

        final Button button = (Button) findViewById(R.id.button_id);
        button.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                try {
                    EditText et = (EditText) findViewById(R.id.edittext);
                    String str = et.getText().toString();
                    ArrayList<String> match = new ArrayList<String>();
                    match.add("D");
                    match.add("B");
                    match.add("C");
                    match.add("CB");
                    match.add("BC");
                    match.add("DB");
                    match.add("BD");
                    match.add("CD");
                    match.add("DC");
                    match.add("BCD");
                    match.add("BDC");
                    match.add("CBD");
                    match.add("CDB");
                    match.add("DBC");
                    if (match.contains(str)) {


                    PrintWriter out = new PrintWriter(new BufferedWriter(
                            new OutputStreamWriter(socket.getOutputStream())),
                            true);
                    out.println(str);
                }

                    else {
                        Toast msg = Toast.makeText(getBaseContext(),"Incorrect Input",Toast.LENGTH_LONG);
                        msg.show();
                    }
                } catch (UnknownHostException e) {
                    e.printStackTrace();
                } catch (IOException e) {
                    e.printStackTrace();
                } catch (Exception e) {
                    e.printStackTrace();
                }

            }
        });

    }



    class ClientThread implements Runnable {
        @Override
        public void run() {
            try {
                InetAddress serverAddr = InetAddress.getByName(SERVER_IP);
                socket = new Socket(serverAddr, SERVERPORT);
            } catch (UnknownHostException e1) {
                e1.printStackTrace();
            } catch (IOException e1) {
                e1.printStackTrace();
            }
        }
    }

    //function to authenticate a digit
    //public void checkDigit(){}
}
