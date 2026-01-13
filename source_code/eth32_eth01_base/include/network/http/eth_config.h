
const char *HTML_CONTENT_CONFIG = R""""(
<html>
    <head>
        <title>Pixel Config Page</title>
        <style>
            #ethernetData{
                display: grid;
                grid-template-columns: 1fr 2fr;
                grid-template-rows: repeat(5, 1fr);
            }
        </style>
        <script lang="Javascript">
            function dhcpValueChanged(){
                let dhcpValue = document.getElementById("dhcp").checked;
                document.getElementById("staticip").disabled  = dhcpValue;
                document.getElementById("staticsub").disabled = dhcpValue;
            }

            function postEthernetData(){
                document.getElementById("ethernetForm").submit();
            }
        </script>
    </head>
    <body>
        <form method="post" id="ethernetForm" action="config/">
            <section id="ethernetData">
                <label>Use DHCP</label><input type="checkbox" name="dhcp" id="dhcp" onchange="dhcpValueChanged()">
                <label>Static IP</label><input type="input" name="staticip" id="staticip" >
                <label>Static Subnet</label><input type="input" name="staticsub" id="staticsub">
                <label>Password</label><input type="password" name="password" id="password">
                <p></p><button id="ethernetSubmit" onclick="postEthernetData()">set Wired Ethernet data</button>
            </section>
        </form>
        
    </body>
</html>
)"""";