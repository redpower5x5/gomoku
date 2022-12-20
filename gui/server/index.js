var express = require('express');
var cors = require('cors');
var exec = require('child_process').exec;

console.log('Start listening...');
start();

function start() {
  const corsOptions = {
    origin: 'http://localhost:8000',
  }

  var app = express();
  app.use(cors());

  app.get('/status', function (req, res) {
    res.send('ok');
  });

  // Compute move
  app.get('/move', function (req, res) {
    // Get query parameters
    var state = req.query.s;
    var player = req.query.p;

    // Build command
    var cmd = 'gomoku';
    if (typeof state !== 'undefined' && state.length > 0) cmd += ' -s ' + state;
    if (typeof player !== 'undefined' && player.length > 0) cmd += ' -p ' + player;

    // Execute command
    exec(cmd, function(error, stdout, stderr) {
      // Write response
      res.write(stdout);
      res.end();
    });
  });
  app.listen(8001);
}
