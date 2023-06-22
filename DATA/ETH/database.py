import csv
import sqlite3
#
# # csv_file1 = 'EFSA_Data_DONE.csv'
# csv_file1 = 'resistancebank_livestock2.csv'
#
#
# # with open(csv_file1, 'rb') as file:
# #     raw_data = file.read()
# #     result = chardet.detect(raw_data)
# #
# # encoding = result['encoding']
# # confidence = result['confidence']
# #
# # print(f"L'encodage du fichier est {encoding} avec une confiance de {confidence * 100}%")
#
#
# with open(csv_file1, 'r') as file:
#     csv_data = csv.reader(file, delimiter="\t")
#     data = list(csv_data)
#
# db_file = 'AMR.db'
# conn = sqlite3.connect(db_file)
# cursor = conn.cursor()
#
# table_name = 'resistancebank'
# # table_name = 'EFSA'
#
# header = data[0]
# create_table_query = f"CREATE TABLE {table_name} ({','.join(header)})"
# create_table_query = create_table_query.replace(".", "_", 1)
# print(create_table_query)
# cursor.execute(create_table_query)
#
# insert_query = f"INSERT INTO {table_name} VALUES ({','.join(['?'] * len(header))});"
# cursor.executemany(insert_query, data[1:])
#
# conn.commit()
#
# conn.close()


# db_file = 'AMR.db'
# conn = sqlite3.connect(db_file)
# cursor = conn.cursor()
#
# table_name = 'EFSA'
#
# # Requête pour obtenir la structure de la table
# query = f"PRAGMA table_info({table_name})"
# cursor.execute(query)
#
# # Récupérer les informations sur les colonnes
# columns = cursor.fetchall()
#
# # Afficher la structure de la table
# for column in columns:
#     print(column)
#
# conn.close()






db_file = 'AMR.db'
conn = sqlite3.connect(db_file)
cursor = conn.cursor()

query = 'SELECT count(*), Year FROM EFSA WHERE Country = "France" AND Animal = "Cattle" AND Pathogen = "Ecoli" GROUP BY Year;'

cursor.execute(query)

# Récupérer tous les enregistrements retournés par la requête
rows = cursor.fetchall()

# Afficher les enregistrements
for row in rows:
    print(row)

conn.close()

