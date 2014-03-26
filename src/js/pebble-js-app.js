var idxs = [3, 4, 5, 6];
function getOpenStr(data) {
  return (data.machinecount - data.inusecount).toString() + " open";
}
function sendMsg() {
  var response;
  //msg.data = "Default value";
  var req = new XMLHttpRequest();
  
  req.open('GET', 'https://my.engr.illinois.edu/labtrack/util_data_json.asp', true);
  
  req.onload = function(e) {
    if(req.readyState == 4) {
      if(req.status == 200) {
        //console.log(req.responseText);
        response = JSON.parse(req.responseText);
        var data = response.data;
        var msg = {};
          
        msg.init = true;
        msg.name0 = data[idxs[0]].strlabname;
        msg.data0 = getOpenStr(data[idxs[0]]);
        msg.name1 = data[idxs[1]].strlabname;
        msg.data1 = getOpenStr(data[idxs[1]]);
        msg.name2 = data[idxs[2]].strlabname;
        msg.data2 = getOpenStr(data[idxs[2]]);
        msg.name3 = data[idxs[3]].strlabname;
        msg.data3 = getOpenStr(data[idxs[3]]);
        
        console.log(msg);
        Pebble.sendAppMessage(msg);
        console.log("Message sent");       
      } else {
        console.log("Request returned error code " + req.status.toString());
      }
    } else {
      console.log("Request in state " + req.readyState);
    }
  };
  
  req.send(null);


}

Pebble.addEventListener("ready",
  function(e) {
    console.log("JavaScript app ready and running!");
    //Pebble.showSimpleNotificationOnPebble('title', 'text');
    sendMsg();

  }
);