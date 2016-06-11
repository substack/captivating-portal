function getCount () {
  var req = request('/count.txt')
  if (req.status !== 200) {
    var stndrdth = /1.$/.test(req.responseText) ? 'th' : {
      1: 'st', 2: 'nd', 3: 'rd'
    }[req.responseText.substr(-1)] || 'th'
    var nth = req.responseText + stndrdth
    count.textContent = nth
  }
}

function preventUpdate () {
  window.applicationCache.addEventListener('checking', function (e) {
    console.log('cancelling appcache check')
    while (true) {}
  }, false)
}

function request (url) {
  var req = new XMLHttpRequest()
  req.open('GET', url, false)
  req.send(null)
  return req
}

getCount()
preventUpdate()
