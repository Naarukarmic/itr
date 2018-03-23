INSERT INTO nourriture VALUES
(1, 'foin', 	'frogs'), 
(2, 'foin', 	'california'),
(3, 'fourrage', 'hopscotch'),
(4, 'foin', 	'tonton'),
(3, 'ble', 		'snapper'),
(1, 'mais', 	'shanghai'),
(5, 'mais', 	'basque');

INSERT INTO exploitation VALUES
(123456, 'isae',	 'GAEC'),
(123126, 'ensica',	 'GAEC'),
(121456, 'supaero',	 'EARL'),
(128956, 'chill',	 'autre'),
(114126, 'foyer', 	 'GAEC');

INSERT INTO exp_nou_rel VALUES
(123456, 'frogs'),
(123456, 'california'),
(123126, 'hopscotch'),
(121456, 'tonton'),
(121456, 'snapper'),
(128956, 'shanghai'),
(128956, 'basque'),
(114126, 'hopscotch'),
(114126, 'frogs'),
(114126, 'snapper');

INSERT INTO eleveur VALUES
(59323, 'lem',	   'rempart',   'toulouse', 123126),
(54223, 'primate', 'rempart',   'toulouse', 114126),
(54223, 'cyril',   'rangueil',  'toulouse', 123456),
(54223, 'quentin', 'rangueil',  'toulouse', 128956),
(54223, 'nono',    'rangueil',  'toulouse', 121456);

INSERT INTO troupeau VALUES
();

INSERT INTO vache VALUES
(1289, 'john', 'limousine', 'bon', 45, troupeau_id),
();