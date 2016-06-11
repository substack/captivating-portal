count.textContent = localStorage.getItem('count') || ''

var req = new XMLHttpRequest
req.open('GET', '/count.txt', false)
req.send(null)

count.textContent = req.responseText
localStorage.setItem('count', count.textContent)
