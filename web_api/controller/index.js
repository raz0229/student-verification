const db = require( '../database/db')
const { doc, getDoc } = require('firebase/firestore');

module.exports = {
    getStudent: async (rollNumber = 0) => {
      let promise = new Promise(async (resolve, reject) => {
        const docRef = doc(db, "students", rollNumber);
        const docSnap = await getDoc(docRef);
        if (docSnap.exists()) {
          resolve(docSnap.data())
        } else {
          resolve({
            "name": "Unknown",
            "created": 0,
            reason: "Unkno"
          })
        }
      })

      return await promise
      
    }
}
