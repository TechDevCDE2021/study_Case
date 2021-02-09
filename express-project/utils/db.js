const { MongoClient } = require('mongodb');
const uri = "mongodb+srv://campus-greenhouse-db:campus123@cluster0.mvmha.mongodb.net/test?retryWrites=true&w=majority"

module.exports = callback => {
    const client = new MongoClient(uri, { useNewUrlParser: true, useUnifiedTopology: true });
    try {
        client.connect();
        callback(client);
    } catch (e) {
        console.error(e);
    } finally {
        client.close();
    }
};