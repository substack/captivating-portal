var req = new XMLHttpRequest
req.open('GET', '/count.txt', false)
req.send(null)

if (req.status === 200) {
  var stndrdth = /1.$/.test(req.responseText) ? 'th' : {
    1: 'st', 2: 'nd', 3: 'rd'
  }[req.responseText.substr(-1)] || 'th'
  var nth = req.responseText + stndrdth
  count.textContent = nth
}
