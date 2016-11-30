//Lets require/import the HTTP module
var http = require('http');

//Lets define a port we want to listen to
const PORT=8080; 

//We need a function which handles requests and send response
function handleRequest(request, response)
{
	console.dir(request.param);

    if (request.method == 'POST') {
        console.log("POST");
        var body = '';
        request.on('data', function (data) {
            body += data;
            console.log("Partial body: " + body);
        });
        request.on('end', function () {
            console.log("Body: " + body);
        });
        response.writeHead(200, {'Content-Type': 'text/html'});
        response.end('post received');
    }
    else
    {
        console.log("GET");
        //var html = '<html><body><form method="post" action="http://localhost:3000">Name: <input type="text" name="name" /><input type="submit" value="Submit" /></form></body>';
        var html = fs.readFileSync('index.html');
        response.writeHead(200, {'Content-Type': 'text/html'});
        response.end(html);
    }

    //response.end('It Works!! Path Hit: ' + request.url);
}

//Create a server
var server = http.createServer(handleRequest);

//Lets start our server
server.listen(process.env.PORT || PORT, function(){
    //Callback triggered when server is successfully listening. Hurray!
    console.log("Server listening on: http://localhost:%s", process.env.PORT || PORT);
});

//LAT=27.714518 LON=-97.328522 SAT=3 PREC=375 CHARS=3227261 SENTENCES=6399 CSUM ERR=1
