package aadu.seniordesign;


import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.telephony.SmsMessage;
import android.widget.Toast;

import java.io.FileOutputStream;

import aadu.seniordesign.NewEntry;

public class SmsListener extends BroadcastReceiver {

    public static final String ACTION = "android.provider.Telephony.SMS_RECEIVED";

    @Override
    public void onReceive(Context context, Intent intent)
    {
        if (intent.getAction().equals(ACTION)){
            Bundle bundle = intent.getExtras();
            if (bundle != null){
                Object[] pdus = (Object[]) bundle.get("pdus");
                SmsMessage[] messages = new SmsMessage[pdus.length];
                for (int i = 0; i < pdus.length; i++){
                    messages[i] = SmsMessage.createFromPdu((byte[]) pdus[i]);
                }
                for (SmsMessage message : messages){

                    String messageBody = message.getDisplayMessageBody();
                    double messageTimestamp = message.getTimestampMillis();
                    if(messageBody.split(":")[0].equals("VPP") || messageBody.split(":")[0].equals("TMP"))
                    {
                        Toast.makeText(context, "Received Senior Design Message", Toast.LENGTH_LONG).show();
                        String fileName = "data.txt";
                        String content = messageBody + ":TIME:" + messageTimestamp + "\n";
                        FileOutputStream outputStream = null;
                        try {
                            outputStream = context.openFileOutput(fileName, Context.MODE_APPEND);
                            outputStream.write(content.getBytes());
                            outputStream.close();
                        } catch (Exception e) {
                            e.printStackTrace();
                        }

                    }
                    else
                    {
                        Toast.makeText(context, "Received Other Message", Toast.LENGTH_LONG).show();
                    }

                }
            }
        }
    }

}