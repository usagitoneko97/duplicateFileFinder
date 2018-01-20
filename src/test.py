import fileDuplication


dupL = fileDuplication.findDuplicate("testFolder")

print(dupL.numberOfDuplication)

# fileDuplication.ptrcast(dupL.list.head.data, fileDuplication.FileProperty())

print(dupL.list.head.data.crc)