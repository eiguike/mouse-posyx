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

var maxX = 5;
var maxY = 2.5;

function handleOrientation(event) {
  var x = event.beta;  // In degree in the range [-180,180]
  var y = event.gamma; // In degree in the range [-90,90]

  if (x >  maxX) { x =  maxX};
  if (x < -maxX) { x = -maxX};

  //x += 90;
  //y += 90;

  command = "MOVE@"+ Math.floor(-y) + "@" + Math.floor(x);
  MOUSE_SOCKET.socket.send(command);
}

// Touchpad code
function mouse_touch_start(e) {
  MOUSE_SOCKET.timer = new Date().getTime();
  window.addEventListener('deviceorientation', handleOrientation);
}

function mouse_touch_end(e) {
  window.removeEventListener("deviceorientation", handleOrientation, false);
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

  //MOUSE_SOCKET.events.left_button.addEventListener("touchstart", mouse_click_left, false);
  //MOUSE_SOCKET.events.left_button.addEventListener("touchend", mouse_release_left, false);

  //MOUSE_SOCKET.events.right_button.addEventListener("touchstart", mouse_click_right, false);
  //MOUSE_SOCKET.events.right_button.addEventListener("touchend", mouse_release_right, false);
}

function mouse_start() {
  mouse_initialize_socket();
  mouse_intialize_events();
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
