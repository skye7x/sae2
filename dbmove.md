db.submissions.find().pretty()

# count rows
db.submissions.countDocuments()

# find by website
db.submissions.find({ website: "example.com" }).pretty()

# find by hostname
db.submissions.find({ hostname: "serverek" }).pretty()

# delete one row by id
db.submissions.deleteOne({ _id: ObjectId("paste_id_here") })

# delete all rows
db.submissions.deleteMany({})

# exit
exit