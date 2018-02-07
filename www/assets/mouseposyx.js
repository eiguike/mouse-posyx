// todo: improve this script below
function ready() {
  var sensivity = 3;
  var touchpad_html = document.querySelector("#touchpad_div");
  var left_html = document.querySelector("#left_click");
  var right_html = document.querySelector("#right_click");
  var last_action = ""
  var timer = new Date();

  touchpad_html.addEventListener("touchstart", touchStart, false);
  touchpad_html.addEventListener("touchend", touchEnd, false);

  left_html.addEventListener("touchstart", leftClickEvent, false);
  left_html.addEventListener("touchend", leftReleaseClickEvent, false);

  right_html.addEventListener("touchstart", rightClickEvent, false);
  right_html.addEventListener("touchend", rightReleaseClickEvent, false);

  function leftReleaseClickEvent(e) {
    socket.send("RELEASE_CLICK@1");
  }

  function leftClickEvent(e) {
    socket.send("CLICK@1");
  }

  function rightClickEvent(e) {
    socket.send("CLICK@3");
  }

  function rightReleaseClickEvent(e) {
    socket.send("RELEASE_CLICK@3");
  }

  position = {
    initial_x:0, initial_y:0
  }

  function touchStart(e) {
    last_action = "";
    e.target.addEventListener("touchmove", touchMoved, false);
    position.initial_x = e.targetTouches[0].clientX;
    position.initial_y = e.targetTouches[0].clientY;

    timer = new Date().getTime();
  }

  function touchEnd(e) {
    e.target.removeEventListener("touchmove", touchMoved, false);
    position.initial_x = 0;
    position.initial_y = 0;

    if (last_action === "") {
      var actual_timer = new Date();
      if (actual_timer.getTime() - timer < 500) {
        leftClickEvent(e);  
        leftReleaseClickEvent(e);  
      }
    }
  }

  function touchMoved(e) {
    final_x = position.initial_x - e.targetTouches[0].clientX;
    final_y = position.initial_y - e.targetTouches[0].clientY;

    //touchpad_html.innerHTML = "X:" + position.initial_x + "  Y:" + position.initial_y;

    position.initial_x = e.targetTouches[0].clientX;
    position.initial_y = e.targetTouches[0].clientY;

    x = Math.floor(final_x * sensivity);
    y = Math.floor(final_y * sensivity);

    if ((x != 0) || (y != 0)){
      socket.send("MOVE@"+ x + "@" + y);
      last_action = "MOVE";
    }
  }

  // connection parts
  var socket = new WebSocket( "ws://" + document.domain + ':' + location.port, "mouse-protocol" );
  //var socket;
  function initialize_socket() {
    ip_address = document.getElementById("ip_address").value;
    socket = new WebSocket("wss://" + ip_address + ":8000", "mouse-protocol");
    socket.onopen = function() { alert("socket open");}
    socket.onclose = function() { alert("socket close");}
    socket.onmessage = function(msg) { alert("socket message: " + msg.data); }
  }

  function stress_test() {
    var limit = 100000;

    for (i=0; i < limit; i++) {
      var value1 = Math.floor(Math.random() * 1920);
      var value2 = Math.floor(Math.random() * 1080);
      socket.send(value1+"@"+value2);
    }
  }


  var socket_keyboard = new WebSocket( "ws://" + document.domain + ':' + location.port, "keyboard-protocol" );
  //socket_keyboard.onopen = function() { console.log("socket open");}
  //socket_keyboard.onclose = function() { console.log("socket close");}
  //socket_keyboard.onmessage = function(msg) { console.log("socket message: " + msg.data); }

  var keyboard_input = document.querySelector("#keyboard_input");
  var compatibility = 0;
  var text_length = 0;
  var output_text = "";

  if (compatibility === 1) {
  } else {
    //keyboard_input.addEventListener("keyup", firefox_working, false);
    //keyboard_input.addEventListener("keypress", test, false);
    keyboard_input.addEventListener("input", test2, false);
    //keyboard_input.addEventListener("keydown", test, false);
  }

  function test2(e) {
    keyboard_input.removeEventListener("keydown", test, false);
    if(text_length < keyboard_input.value.length){
      socket_keyboard.send("PRESS@"+keyboard_input.value.slice(-1));
    } else if (text_length > keyboard_input.value.length) {
      socket_keyboard.send("PRESS@"+"Backspace");
    }

    text_length = keyboard_input.value.length;

    if (text_length === 0) {
      keyboard_input.addEventListener("keydown", test, false);
    }
  }

  function test(e){
    platform = document.querySelector("#log");
    platform.innerHTML = e.key;
    if((text_length === 0) || (e.key === "Backspace")){
      socket_keyboard.send("PRESS@"+"Backspace"+"@");
    }
  }
}

document.addEventListener ("DOMContentLoaded", ready);



