var express = require('express');
var bodyParser = require('body-parser');
var twilio = require('twilio');
var client = new twilio.RestClient('AC27415e72127ed4b9cddb944f188a81ef', '918ef45e3ba1e5049c239435b6a28bb3');
var Request = require('request');
var app = express();

app.use(bodyParser.json());
app.use(bodyParser.urlencoded(
{
	extended: true
}));

app.set('port', (process.env.PORT || 8080));
app.use(express.static(__dirname + '/public'));


app.get('/', function(request, response)
{
	response.send("Why Hello There...");
});

app.post('/', function(request, response)
{
	console.log(request.body);
	response.send(request.body);

	var phoneNumber = request.body.number;
	var latitude = request.body.latitude;
	var longititude = request.body.longititude;

	var resp = new twilio.TwimlResponse();

	//getUber();

	var newDateObj = new Date(new Date().getTime() + 15 * 60000);

	Request('http://api.geonames.org/findNearestAddressJSON?lat=' + latitude + '&lng=' + longititude + '&username=palagerini', function (error, response, body) 
	{
  		if(!error)
  		{
  			console.log(body);

  			var street = body.street;
  			var streetNumber = body.streetNumber;

  			
  			app.get('/whatdo', function(request, response)
  			{
  				resp.say({voice:'woman'}, "Your Uber driver will pick you up at " + streetNumber + " " + street + " at " + newDateObj.toLocaleString());
  				response.writeHead(200, 
  				{
  					'Content-Type':'text/xml'
  				});
  				response.end(resp.toString());
  			})

  			client.makeCall(
  			{
  				to:phoneNumber,
  				//to:'+13612441060',
  				from:'+12569739465',
  				url: 'http://elderly-mobile.herokuapp.com/whatdo'
  			}, function(err, responseData) 
  			{

    			//executed when the call has been initiated.
    			//console.log(responseData.from); // outputs "+14506667788"
    			console.log(err);
			});

  		}
	});

});

app.listen(app.get('port'), function()
	{
		console.log("Now listening on port " + app.get('port'));
	});


//LAT=27.714518 LON=-97.328522 SAT=3 PREC=375 CHARS=3227261 SENTENCES=6399 CSUM ERR=1
//{"number": "+13616882314", "latitude": "27.714518", "longititude": "-97.328522"}
//elderly-mobile.herokuapp.com