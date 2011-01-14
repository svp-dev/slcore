def funfilter(filter, items):
    for (type, content, comment) in items:
        if type == 'fun':
            #print "XXXXXXXXXXXXXXXXXXXXXX"
            #print filter
            #pprint.pprint(content)
            content['body'] = [x for x in filter(content, content['body'])]
            #pprint.pprint(content)
        yield (type, content, comment)

def cfunfilter(filter, items):
    for (type, content, comment) in items:
        if type == 'cfun':
            #print "XXXXXXXXXXXXXXXXXXXXXX"
            #print filter
            #pprint.pprint(content)
            content['body'] = [x for x in filter(content, content['body'])]
            #pprint.pprint(content)
        yield (type, content, comment)

