require('./web');
const db = require('./db');

db(client => {
    const ttn = require('./ttn');
    console.log(ttn.isSending);
    ttn(client);
});




