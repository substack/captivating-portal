function getCount () {
  var req = request('/count.txt')
  if (req.status === 200) {
    var stndrdth = /1.$/.test(req.responseText) ? 'th' : {
      1: 'st', 2: 'nd', 3: 'rd'
    }[req.responseText.substr(-1)] || 'th'
    var nth = req.responseText + stndrdth
    count.textContent = nth
  }
}

function preventUpdate () {
  console.log('checking if we are on a real network')
  var req = request('/check.txt')
  if (req.status === 200) {
    console.log('not on real network')
    return
  }

  console.log('on real network, preventing future appcache checks')
  window.applicationCache.addEventListener('checking', function (e) {
    console.log('preventing appcache check')
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
