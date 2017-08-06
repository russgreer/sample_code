package com.insomtech.theapp;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.text.InputType;
import android.view.View;
import android.view.WindowManager;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.Locale;

 
public class FirstSignupActivity extends AppCompatActivity {

    private Button nextButton;
    private EditText username, email, password, city, zip, age;
    private Spinner country_spinner, state_spinner;
    String userid;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // Get userid from last screen (AddPicActivity)
        Intent intent = getIntent();
        userid = intent.getStringExtra("userid");

        setContentView(R.layout.activity_first_signup);

        // get handles to the text boxes...
        username   = (EditText)findViewById(R.id.username);
        email      = (EditText)findViewById(R.id.email);
        password   = (EditText)findViewById(R.id.password);
        city       = (EditText)findViewById(R.id.city);
        zip        = (EditText)findViewById(R.id.zip);
        age        = (EditText)findViewById(R.id.age);

        country_spinner = (Spinner) findViewById(R.id.country_spinner);
        state_spinner   = (Spinner) findViewById(R.id.state_spinner);

        age.setInputType(InputType.TYPE_CLASS_NUMBER );

        // Setup the next button...
        nextButton = (Button)findViewById(R.id.next_button);

        nextButton.setOnClickListener( new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                FirstSignupActivity.this.onNextButtonClick(v);
            }
        });

        // turn off the keyboard initially...
        getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_STATE_HIDDEN);

        // setup the countries drop down list...
        ArrayList<String> country = new ArrayList<String>();

        country.add("Choose your country");
        country.add("United states");

        // setup the country spinner (drop down)
        String[] isoCountries = Locale.getISOCountries();
        int i = 0;
        for (String a_country : isoCountries) {
            Locale locale = new Locale("en", a_country);

            country.add(locale.getDisplayCountry());
           }

        ArrayAdapter<String> country_stringArrayAdapter=
                new ArrayAdapter<String>(this,
                        android.R.layout.simple_spinner_dropdown_item,
                        country);
        Spinner spinner = (Spinner)  findViewById(R.id.country_spinner);
        spinner.setAdapter(country_stringArrayAdapter);


        // setup the states drop down list... Should only come up if America is the country FIX later
        String[] state = { "Choose your state", "Alabama","Alaska","Arizona","Arkansas","California",
                "Colorado","Connecticut","Delaware","Florida","Georgia","Hawaii","Idaho",
                "Illinois","Indiana","Iowa","Kansas","Kentucky","Louisiana","Maine","Maryland",
                "Massachusetts","Michigan","Minnesota","Mississippi","Missouri","Montana",
                "Nebraska","Nevada","New Hampshire","New Jersey","New Mexico","New York",
                "North Carolina","North Dakota","Ohio","Oklahoma","Oregon","Pennsylvania",
                "Rhode Island","South Carolina","South Dakota","Tennessee","Texas","Utah",
                "Vermont","Virginia","Washington","West Virginia","Wisconsin","Wyoming" };

        ArrayAdapter<String> state_stringArrayAdapter=
                new ArrayAdapter<String>(this,
                        android.R.layout.simple_spinner_dropdown_item,
                        state);
        spinner = (Spinner)  findViewById(R.id.state_spinner);
        spinner.setAdapter(state_stringArrayAdapter);

    }


    // next button clicked...
    public void onNextButtonClick(View view){

        // Check that all fields have been filled out and that the spinners have
        // been changed from the defaults...
        if( username.getText().toString().isEmpty() ||
            email.getText().toString().isEmpty() ||
            password.getText().toString().isEmpty() ||
            city.getText().toString().isEmpty() ||
            zip.getText().toString().isEmpty() ||
            age.getText().toString().isEmpty() )
        {
            Toast.makeText(getApplicationContext(), "Please fill in all the text boxes.", Toast.LENGTH_SHORT).show();
            return;
        }

        if( country_spinner.getSelectedItem().toString().equals("Choose your country") )
        {
            Toast.makeText(getApplicationContext(), "Please choose a country.", Toast.LENGTH_SHORT).show();
            return;
        }


        if( state_spinner.getSelectedItem().toString().equals("Choose your state") )
        {
            Toast.makeText(getApplicationContext(), "Please choose a state.", Toast.LENGTH_SHORT).show();
            return;
        }

        // go to the next screen (my Tribe screen)
        Intent i = new Intent(FirstSignupActivity.this, MyTribeActivity.class); // change too class...

        // Package all the info needed for the next screen and pass it...

        i.putExtra( "userid", userid ); // pass uid (unique to this phone)
        i.putExtra( "username", username.getText().toString());
        i.putExtra( "email", email.getText().toString());
        i.putExtra( "password", password.getText().toString());
        i.putExtra( "city", city.getText().toString());
        i.putExtra( "zip", zip.getText().toString());
        i.putExtra( "age", age.getText().toString());
        i.putExtra( "country", country_spinner.getSelectedItem().toString());
        i.putExtra( "state", state_spinner.getSelectedItem().toString());

        startActivity(i);
    }
}
