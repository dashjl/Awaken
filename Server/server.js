/**
 * Created by JohnL on 3/13/2017.
 */
var express = require('express');
var app = express();
var http = require('http').Server(app);
var io = require('socket.io')(http);
app.use(express.static('public'));

var nextState = [1,0,3,2];
var devices = {1:0,2:0,3:0,4:0};
var update = {device:0,state:0};
var users = 0;

var schedule = require('node-schedule');
var rule = new schedule.RecurrenceRule();

rule.minute = 6;
var s1 = schedule.scheduleJob(rule, function(){
    console.log("ALARM, Light 1 ON");
    devices[1]=1;
    update["device"]=1;
    update["state"]=devices[1];
    io.sockets.emit('update', update);
});
rule.minute=7;
var s2 = schedule.scheduleJob(rule, function(){
    console.log("ALARM, Light 2 ON");
    devices[2]=1;
    update["device"]=2;
    update["state"]=devices[2];
    io.sockets.emit('update', update);
});
rule.minute=8;
var s3 = schedule.scheduleJob(rule, function(){
    console.log("ALARM, Light 3 ON");
    devices[3]=1;
    update["device"]=3;
    update["state"]=devices[3];
    io.sockets.emit('update', update);
});
rule.minute=9;
var s4 = schedule.scheduleJob(rule, function(){
    console.log("ALARM, Light 4 ON");
    devices[4]=1;
    update["device"]=4;
    update["state"]=devices[4];
    io.sockets.emit('update', update);
});

io.on('connection', function(socket) {
    console.log("Users:"+users++);
    //Onload, push 4 updates, one for each device
    for(i=1;i<5;i++){
        update["device"]=i;
        update["state"]=devices[i];
        io.sockets.emit('update', update);
    }
    //socket.emit('update', update);
    socket.on('disconnect', function(){
        console.log("Users:"+users--);
    });
    socket.on('login', login.bind(null,socket));
    socket.on('toggle', toggle);
});
http.listen(3000, function() {
    console.log('listening on *:3000');
});
function login(socket,data){
    if(data.username==0){
        socket.join('Client');
        console.log(socket.id + " is a web client");
    }
    else if(data.username==1){
        socket.join('Devices');
        console.log(socket.id+" is a device");
    }
}
function toggle(data){
    id=data.id;
    devices[id]=nextState[devices[id]];
    console.log('light: ' + id +" state: " + devices[id]);
    update["device"]=id;
    update["state"]=devices[id]
    io.sockets.emit('update', update);
}
