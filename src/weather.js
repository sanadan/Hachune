var xhrRequest = function ( url, type, callback )
{
  var xhr = new XMLHttpRequest();
  xhr.onload = function ()
  {
    callback( this.responseText );
  };
  xhr.open( type, url );
  xhr.send();
};

function locationSuccess( pos )
{
  // Construct URL
  var url = 'http://api.openweathermap.org/data/2.5/weather?APPID=4ccdb10b7a744c7fef9487b83e8be5d4&lat=' +
      pos.coords.latitude + '&lon=' + pos.coords.longitude;

  // Send request to OpenWeatherMap
  xhrRequest( url, 'GET', 
    function( responseText )
    {
      // responseText contains a JSON object with weather info
      var json = JSON.parse( responseText );
      
      // Temperature in Kelvin requires adjustment
      var temperature = Math.round( json.main.temp - 273.15 );
      console.log( 'Temperature is ' + temperature );

      // Conditions
      var conditions = json.weather[ 0 ].main;      
      console.log( 'Conditions are ' + conditions );
      
      // Wind speed
      var wind_speed = json.wind.speed;
      console.log( 'Wind speed is ' + wind_speed ) ;
      
      // City
      var city = json.name;

      // Assemble dictionary using our keys
      var dictionary =
      {
        'WEATHER_CONDITION': conditions,
        'WEATHER_TEMPERATURE': temperature,
        'WEATHER_WIND_SPEED' : String( wind_speed ),
        'WEATHER_CITY' : city,
      };
      
      // Send to Pebble
      Pebble.sendAppMessage( dictionary,
        function ( e )
        {
          console.log( 'Weather info sent to Pebble successfully!' );
        },
        function ( e )
        {
          console.log( 'Error sending weather info to Pebble!' );
        }
      );
    }
  );
}

function locationError( err )
{
  console.log( 'Error requesting location!' );
}

function getWeather()
{
  navigator.geolocation.getCurrentPosition(
    locationSuccess,
    locationError,
    { timeout: 15000, maximumAge: 60000 }
  );
}

// Listen for when the watchface is opened
Pebble.addEventListener( 'ready', 
  function ( e )
  {
    console.log( 'Weather.js ready!' );

    // Get the initial weather
    getWeather();
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener( 'appmessage',
  function ( e )
  {
    console.log( 'AppMessage received!' );

    getWeather();
  }
);
