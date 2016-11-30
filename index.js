var express = require('express');
var bodyParser = require('body-parser');
var app = express();

app.set('port', (process.env.PORT || 8080));
app.use(express.static(__dirname + '/public'));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded(
{
	extended: true
}));

app.get('/', function(request, response)
{
	response.send("Why Hello There...");
});

app.post('/', function(request, response)
{
	console.log(request.body.name);
	response.send(request.body);
});

app.listen(app.get('port'), function()
	{
		console.log("Now listening on port " + app.get('port'));
	});


//LAT=27.714518 LON=-97.328522 SAT=3 PREC=375 CHARS=3227261 SENTENCES=6399 CSUM ERR=1
