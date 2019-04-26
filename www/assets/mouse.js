var MouseSocket = function() {
  return {
    position: { initial_x: 0, initial_y: 0 },
    sensitivity: 3,
    last_action: "START_TOUCH",
    timer: new Date(),
    events: {},
//    stress: mouse_stress_test,
    start: mouse_start
  }
}

// Touchpad code
function mouse_touch_start(e) {
  MOUSE_SOCKET.last_action = "";
  e.target.addEventListener("touchmove", mouse_touch_move, false);

  MOUSE_SOCKET.position.initial_x = e.targetTouches[0].clientX;
  MOUSE_SOCKET.position.initial_y = e.targetTouches[0].clientY;

  MOUSE_SOCKET.timer = new Date().getTime();
}

function mouse_touch_end(e) {
  e.target.removeEventListener("touchmove", mouse_touch_move, false);
  MOUSE_SOCKET.position.initial_x = 0;
  MOUSE_SOCKET.position.initial_y = 0;

  if (MOUSE_SOCKET.last_action === "") {
    var actual_timer = new Date();
    if (actual_timer.getTime() - MOUSE_SOCKET.timer < 500) {
      mouse_click_left(e);  
      mouse_release_left(e);  
    }
  }
}

function mouse_touch_move(e) {
  final_x = MOUSE_SOCKET.position.initial_x - e.targetTouches[0].clientX;
  final_y = MOUSE_SOCKET.position.initial_y - e.targetTouches[0].clientY;

  //touchpad_html.innerHTML = "X:" + position.initial_x + "  Y:" + position.initial_y;

  MOUSE_SOCKET.position.initial_x = e.targetTouches[0].clientX;
  MOUSE_SOCKET.position.initial_y = e.targetTouches[0].clientY;

  x = Math.floor(final_x * MOUSE_SOCKET.sensitivity);
  y = Math.floor(final_y * MOUSE_SOCKET.sensitivity);

  if ((x != 0) || (y != 0)){
    MOUSE_SOCKET.socket.send("MOVE@"+ x + "@" + y);
    MOUSE_SOCKET.last_action = "MOVE";
  }
}

// Click mouses
function mouse_release_left() {
  MOUSE_SOCKET.socket.send("RELEASE_CLICK@1");
}

function mouse_click_left() {
  MOUSE_SOCKET.socket.send("CLICK@1");
}

function mouse_click_right() {
  MOUSE_SOCKET.socket.send("CLICK@3");
}

function mouse_release_right() {
  MOUSE_SOCKET.socket.send("RELEASE_CLICK@3");
}

function mouse_initialize_socket() {
  if (location.port == "") {
    string_port = ""
  } else {
    string_port = ":" + location.port;
  }

  MOUSE_SOCKET.socket = new WebSocket( "ws://" + document.domain + string_port, "mouse-protocol" );
}

function mouse_intialize_events() {
  MOUSE_SOCKET.events.touchpad = document.querySelector("#touchpad_div");
  MOUSE_SOCKET.events.left_button = document.querySelector("#left_click");
  MOUSE_SOCKET.events.right_button = document.querySelector("#right_click");

  MOUSE_SOCKET.events.touchpad.addEventListener("touchstart", mouse_touch_start, false);
  MOUSE_SOCKET.events.touchpad.addEventListener("touchend", mouse_touch_end, false);

  MOUSE_SOCKET.events.left_button.addEventListener("touchstart", mouse_click_left, false);
  MOUSE_SOCKET.events.left_button.addEventListener("touchend", mouse_release_left, false);
  
  MOUSE_SOCKET.events.right_button.addEventListener("touchstart", mouse_click_right, false);
  MOUSE_SOCKET.events.right_button.addEventListener("touchend", mouse_release_right, false);
}

// Gyroscope Pointer
function mouse_initialize_track_pointer() {
  MOUSE_SOCKET.events.gyroscope_button = document.querySelector(".gyroscope-button");

  MOUSE_SOCKET.events.touchpad.addEventListener("touchstart", mouse_touch_track_pointer_start, false);
  MOUSE_SOCKET.events.touchpad.addEventListener("touchend", mouse_touch_track_pointer_end, false);
}

function mouse_touch_track_pointer_start() {
  window.addEventListener('deviceorientation', mouse_touch_track_pointer_move);
}

function mouse_touch_track_pointer_end() {
  window.removeEventListener("deviceorientation", mouse_touch_track_pointer_move, false);
}

function mouse_touch_track_pointer_move(event) {
  var maxX = 5;
  var maxY = 2.5;
  var x = event.beta;
  var y = event.gamma;

  if (x >  maxX) { x =  maxX};
  if (x < -maxX) { x = -maxX};

  command = "MOVE@"+ Math.floor(-y) + "@" + Math.floor(x);
  MOUSE_SOCKET.socket.send(command);
}

function mouse_start() {
  mouse_initialize_socket();
  mouse_intialize_events();
  mouse_initialize_track_pointer();
}

function mouse_stress_test() {
  var limit = 100000;

  for (i=0; i < limit; i++) {
    var value1 = Math.floor(Math.random() * 1920);
    var value2 = Math.floor(Math.random() * 1080);
    MOUSE_SOCKET.socket.send("MOVE@"+value1+"@"+value2);
  }

  mouse_click_left();
  mouse_release_left();
  mouse_click_right();
  mouse_release_right();
}
