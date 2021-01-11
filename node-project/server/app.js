require('./web');
const db = require('./db');
db(client => {
    const ttn = require('./ttn');
    ttn(client);
});