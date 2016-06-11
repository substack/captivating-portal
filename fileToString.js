#!?usr/bin/env node

var fs = require('fs')
var data = fs.readFileSync(process.argv[2]).toString('utf8')
var lines = data.split('\n')
for (var line of lines) {
  console.log(`  "${line.replace(/"/g, '\\"')}\\n"`)
}
