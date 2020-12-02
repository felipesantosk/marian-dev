// Enables setting runtime args via the query string:
// Module["arguments"] = window.location.search.substr(1).split('%20')
Module["arguments"] = "-m /repo/models/model.npz -v /repo/models/vocab.esen.spm /repo/models/vocab.esen.spm --cpu-threads 1".split(' ');
console.log('Module["arguments"]', Module["arguments"]);
var initStdInOutErr = function() {
  var input = "Hola mundo\n";
  var i = 0;
  function stdin() {
    if (i < input.length) {
      var code = input.charCodeAt(i);
      ++i;
      console.log("STDIN: Feeding character code to stdin: ", code);
      return code;
    } else {
      console.log("STDIN: Done feeding input via stdin: ", input);
      return null;
    }
  }

  var stdoutBuffer = "";
  function stdout(code) {
    if (code === "\n".charCodeAt(0) && stdoutBuffer !== "") {
      console.log("STDOUT: ", stdoutBuffer);
      stdoutBufer = "";
    } else {
      stdoutBuffer += String.fromCharCode(code);
    }
  }

  var stderrBuffer = "";
  function stderr(code) {
    if (code === "\n".charCodeAt(0) && stderrBuffer !== "") {
      console.log("STDERR: ", stderrBuffer);
      stderrBuffer = "";
    } else {
      stderrBuffer += String.fromCharCode(code);
    }
  }

  FS.init(stdin, stdout, stderr);
}
Module["preRun"].push(initStdInOutErr);
