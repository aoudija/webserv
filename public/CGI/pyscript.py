import cgi
import os

# Set content type to HTML
print("Content-type: text/html; charset=UTF-8\r\n")
# Get form data
form = cgi.FieldStorage()
# Get request method
request_method = os.environ.get('REQUEST_METHOD', 'GET')
if (request_method == "GET"):
    names = form.getlist('name')
    last_names = form.getlist('last_name')
    title = "PYTHON GET"
    req = "get"
    if (not(len(names) and len(last_names))):
        print("Status: 302 Found\r\n")
        print("Location: pyget.html\r\n")
        print("\r\n\r\n")
        exit()
else:
    names = form.getlist('namepost')
    last_names = form.getlist('last_namepost')
    title = "PYTHON POST"
    req = "post"
    if (not(len(names) and len(last_names))):
        last_names = form.getlist('last_name')
        print("Status: 302 Found\r\n")
        print("Location: pypost.html\r\n")
        print("\r\n\r\n")
        exit()
# Start HTML response
print("\r\n\r\n")
print("<!DOCTYPE html>")
print("<html lang='en'>")
print("<head>")
print("    <meta charset='UTF-8'>")
print("    <meta name='viewport' content='width=device-width, initial-scale=1.0'>")
print("    <link rel='stylesheet' href='style.css'>")
print("    <title>{}</title>".format(title))
print("</head>")
print("<body>")
print("    <nav class='navMenu'>")
print("        <a href='/CGI/'>Home</a>")
print("        <a href='pyget.html'>PYTHON/GET</a>")
print("        <a href='pypost.html'>PYTHON/POST</a>")
print("    </nav>")
print("    <div class='container'>")
print("        <h1>{}</h1>".format(title))
print("        <p id='php'>First Name: <span>{}</span></p>".format(", ".join(names)))
print("        <p id='php'>Last Name : <span>{}</span></p>".format(", ".join(last_names)))
print("        <form  method={}>".format(req))
print("            <input type='submit' name='logout' value='GO BACK'>")
print("        </form>")
print("    </div>")
print("</body>")
print("</html>")

# import  requests
# requests.get("http://localhost:8080/CGI/pyscript.py")