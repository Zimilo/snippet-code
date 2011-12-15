import web
from config.Router import *

web.config.debug = True

render = web.template.render('templates/')
app = web.application(Router(), globals())

if __name__ == "__main__":
    app.run()
