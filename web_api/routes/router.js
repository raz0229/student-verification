const express = require('express')
const router = express.Router()

const db = require('../controller/index')

router.get('/:roll', async (req, res) => {
  let r = req.params.roll
  if (!r) return res.sendStatus(403)
  const data = await db.getStudent(r)
  return res.status(200).json(data)
})
 

module.exports = router
