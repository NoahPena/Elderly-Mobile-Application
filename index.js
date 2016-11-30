var express = require('express');
var app = express();

app.set('port', (process.env.PORT || 8080));
app.use(express.static(__dirname + '/public'));

app.get('/', function(request, response)
{
	response.send("Why Hello There...");
});

app.post('/', function(request, response)
{
	console.log(request.body);
});

app.listen(app.get('port'), function()
	{
		console.log("Now listening on port " + app.get('port'));
	});

// //Lets require/import the HTTP module
// var http = require('http');

// //Lets define a port we want to listen to
// const PORT=8080; 

// //We need a function which handles requests and send response
// function handleRequest(request, response)
// {
// 	response.writeHead(200, {"Content-Type":"text\plain"});

// 	if(request.method == "GET")
//     {
//             response.end("received GET request.")
//     }
//     else if(request.method == "POST")
//     {
//             response.end("received POST request.");
//     }
//     else
//     {
//             response.end("Undefined request .");
//     }
// }

// //Create a server
// var server = http.createServer(handleRequest);

// //Lets start our server
// server.listen(process.env.PORT || PORT, function(){
//     //Callback triggered when server is successfully listening. Hurray!
//     console.log("Server listening on: http://localhost:%s", process.env.PORT || PORT);
// });

//LAT=27.714518 LON=-97.328522 SAT=3 PREC=375 CHARS=3227261 SENTENCES=6399 CSUM ERR=1
