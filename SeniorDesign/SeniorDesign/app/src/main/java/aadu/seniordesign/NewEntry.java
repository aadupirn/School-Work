package aadu.seniordesign;

import android.content.Context;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.text.method.ScrollingMovementMethod;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;

public class NewEntry extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {


        super.onCreate(savedInstanceState);


        setContentView(R.layout.activity_new_entry);


        Button displayData = (Button) findViewById(R.id.displayData);
        displayData.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Context context = getApplicationContext();
                String file = "data.txt";
                FileInputStream fin = null;
                try {
                    fin = openFileInput(file);
                    int c;
                    String temp="";
                    int msgCounter = 1;
                    while( (c = fin.read()) != -1){
                        temp = temp + Character.toString((char)c);
                    }

                    String[] messages = temp.split("\n");
                    String displayString = "";
                    for(String message: messages)
                    {
                        String timeIn = message.split("TIME")[1];
                        String data = message.split("TIME")[0];
                        long tr = Double.valueOf(timeIn).longValue();

                        DateFormat formatter = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
                        Date dateToDisplay = new Date(tr);
                        String dateString = formatter.format(dateToDisplay);
                        String dataString = "";
                        if(data.split(":")[0].equals("VPP"))
                        {
                            dataString = "\tVPP 1: " + data.split(":")[1].split(",")[0] + "\n\tVPP 2: " + data.split(":")[1].split(",")[1] + "\n\tVPP 3: " + data.split(":")[1].split(",")[2] + "\n\n";
                        }
                        else
                        {
                            dataString = "\tTemperature: " + data.split(":")[1] + "\u00b0F\n\n";
                        }
                        displayString = displayString + "Time: " + dateString + "\n" + dataString;
                        msgCounter++;

                    }

                    TextView dataText = (TextView) findViewById(R.id.dataText);
                    dataText.setText(displayString);
                    dataText.setMovementMethod(new ScrollingMovementMethod());
                } catch (Exception e) {
                    e.printStackTrace();
                    Toast.makeText(context, e.getMessage() + e.getLocalizedMessage() + e.getCause() + e.getStackTrace() + e.toString(), Toast.LENGTH_LONG).show();
                }

            }
        });

        Button fab = (Button) findViewById(R.id.clearData);
        fab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Context context = getApplicationContext();
                String fileName = "data.txt";
                String content = "";
                FileOutputStream outputStream = null;
                try {
                    outputStream = context.openFileOutput(fileName, Context.MODE_PRIVATE);
                    outputStream.write(content.getBytes());
                    outputStream.close();
                } catch (Exception e) {
                    e.printStackTrace();
                    Toast.makeText(context, e.getMessage(), Toast.LENGTH_LONG).show();
                }


            }
        });


    }


    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }
}
