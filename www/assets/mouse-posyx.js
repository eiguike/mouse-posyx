function ready() {
  function mouse_posyx_start() {
    MOUSE_SOCKET = new MouseSocket();
    KEYBOARD_SOCKET = new KeyboardSocket();

    MOUSE_SOCKET.start();
    KEYBOARD_SOCKET.start();
  }

  var MousePosyx = function() {
    return {
      start: mouse_posyx_start
    }
  }

  new MousePosyx().start();
}

document.addEventListener ("DOMContentLoaded", ready);
