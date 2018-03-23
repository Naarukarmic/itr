
-- Drop the tables if they exist

DROP TABLE IF EXISTS vache;
DROP TABLE IF EXISTS troupeau;
DROP TABLE IF EXISTS eleveur;
DROP TABLE IF EXISTS exploitation;
DROP TABLE IF EXISTS exp_nou_rel;
DROP TABLE IF EXISTS nourriture;

-- Create tables

CREATE TABLE IF NOT EXISTS nourriture (
	poids INTEGER,
	nature VARCHAR(20),
	lieu VARCHAR(20) NOT NULL,
	PRIMARY KEY (lieu)
);

CREATE TABLE IF NOT EXISTS exploitation (
	num INTEGER NOT NULL,
	nom VARCHAR(20),
	type VARCHAR(20),
	PRIMARY KEY (num)
);

CREATE TABLE IF NOT EXISTS exp_nou_rel (
	exploitation_id INTEGER NOT NULL,
	nourriture_id VARCHAR(20) NOT NULL,
	PRIMARY KEY (exploitation_id, nourriture_id),
	FOREIGN KEY (exploitation_id) REFERENCES exploitation(num),
	FOREIGN KEY (nourriture_id) REFERENCES nourriture(lieu)

);

CREATE TABLE IF NOT EXISTS eleveur (
	num_soc INTEGER NOT NULL,
	nom VARCHAR(20),
	adresse VARCHAR(20),
	commune VARCHAR(20),
	exploitation_id INTEGER,
	PRIMARY KEY (num_soc),
	FOREIGN KEY (exploitation_id) REFERENCES exploitation(num)
);


CREATE TABLE IF NOT EXISTS troupeau (
	pre VARCHAR(20) NOT NULL,
	chien VARCHAR(20),
	eleveur_id INTEGER,
	PRIMARY KEY (pre),
	FOREIGN KEY (eleveur_id) REFERENCES eleveur(num_soc)
);


CREATE TABLE IF NOT EXISTS vache (
	id INTEGER NOT NULL,
	nom VARCHAR(20),
	race VARCHAR(20),
	rang VARCHAR(20),
	production VARCHAR(20),
	troupeau_id VARCHAR(20),
	PRIMARY KEY (id),
	FOREIGN KEY (troupeau_id) REFERENCES troupeau(pre)
);



