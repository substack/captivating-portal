count.textContent = localStorage.getItem('count') || ''

var req = new XMLHttpRequest
req.open('GET', '/count.txt', false)
req.send(null)

if (req.status === 200) {
  count.textContent = req.responseText
  console.log('SET', req.responseText)
  localStorage.setItem('count', req.responseText)
}
