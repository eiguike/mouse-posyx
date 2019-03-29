var KEYBOARD_SOCKET = KEYBOARD_SOCKET || {}

var KeyboardSocket = function() {
  return {
    events: {},
    text_length: 0,
    //stress: keyboard_stress_test,
    start: keyboard_start 
  }
}

function keyboard_keypress_backspace(e){
  if((KEYBOARD_SOCKET.text_length === 0) || (e.key === "Backspace")){
    KEYBOARD_SOCKET.socket.send("PRESS@"+"Backspace"+"@");
  }
}

function keyboard_keypress(e) {
  keyboard_input = KEYBOARD_SOCKET.events.keyboard_input;

  keyboard_input.removeEventListener("keydown", keyboard_keypress_backspace, false);
  if(KEYBOARD_SOCKET.text_length < keyboard_input.value.length){
    KEYBOARD_SOCKET.socket.send("PRESS@"+keyboard_input.value.slice(-1));
  } else if (KEYBOARD_SOCKET.text_length > keyboard_input.value.length) {
    KEYBOARD_SOCKET.socket.send("PRESS@"+"Backspace");
  }

  KEYBOARD_SOCKET.text_length = keyboard_input.value.length;

  if (KEYBOARD_SOCKET.text_length === 0) {
    keyboard_input.addEventListener("keydown", keyboard_keypress_backspace, false);
  }
}

function keyboard_initialize_events() {
  KEYBOARD_SOCKET.events.keyboard_input = document.querySelector("#keyboard_input");
  KEYBOARD_SOCKET.events.keyboard_input.addEventListener("input", keyboard_keypress, false);
}

function keyboard_initialize_sockets() {
  if (location.port == "") {
    string_port = ""
  } else {
    string_port = ":" + location.port;
  }

  KEYBOARD_SOCKET.socket = new WebSocket( "ws://" + document.domain + string_port, "keyboard-protocol" );
}

function keyboard_start() {
  keyboard_initialize_sockets();
  keyboard_initialize_events();
}


