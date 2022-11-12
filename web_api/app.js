
const express = require( "express")
const bodyParser = require( "body-parser")
const helmet = require('helmet')
const router = express.Router()
const app = express();

app.use(bodyParser.urlencoded({ extended: false }));
app.use(bodyParser.json());
app.use(helmet())

app.use(function (req, res, next) {
  res.setHeader('Access-Control-Allow-Origin', '*'); // allow * domains
  res.setHeader('Access-Control-Allow-Methods', 'GET'); // allow request methods
  res.setHeader('Access-Control-Allow-Headers', 'X-Requested-With,content-type,x-api-key'); // allow headers
  next(); // Pass to next layer of middleware
});

app.use("/", router);
app.use(require('./routes/router'))

app.listen(process.env.PORT || 3000 , () => {
  console.log("FAST Gatepass API running");
});
