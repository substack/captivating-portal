var http = require('http')
var ecstatic = require('ecstatic')
var fs = require('fs')
var count = 0
var countFile = __dirname + '/public/count.txt'
try { count = Number(fs.readFileSync(countFile)) }
catch (err) {}
if (isNaN(count)) count = 0
count = Math.floor(count)
if (count === 0) fs.writeFileSync(countFile, String(count))

var st = ecstatic({
  root: __dirname + '/public',
  cache: 9999999999999999999
})
http.createServer(function (req, res) {
  if (req.url === '/') {
    fs.writeFile(countFile, String(++count), function (err) {
      st(req, res)
    })
  } else st(req, res)
}).listen(Number(process.argv[2]) || 80)
