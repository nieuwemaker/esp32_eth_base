
const char *HTML_CONTENT_PERFORMANCE = R""""(
<html>
    <head>
        <title>Pixel Home Page</title>
        <style>
            
        </style>
        <script src="rqstperform.js"></script>
        <script lang="Javascript">
            //let data = {Cycles:50};
            function writeData(){
                let strData = "";
                strData += "Cycles:" + data.Cycles + "<br>";
                document.getElementById("data").innerHTML = strData;
            }
        </script>
    </head>
    <body onload="writeData()">
        <h1>PERFORMANCE</h1>
        <section id="data"></section>
    </body>
</html>
)"""";