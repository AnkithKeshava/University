import sys
import time
from selenium import webdriver
from selenium.webdriver.common.keys import Keys
from selenium.common.exceptions import NoSuchElementException

def word_test(driver, url, path):
	driver.get(url)

	try:
		elements = (driver.find_elements_by_xpath(path))
	except NoSuchElementException as error:
		print error
		sys.exit()

	words = []
	long_words = []
	long_word_present = False

	for element in elements:
		words.extend(element.text.split())

	for word in words:
		if (len(word) > 10):
			long_words.append(word)
                        long_word_present = True

	try:
        	assert (long_word_present)
	except AssertionError:
		print "Word longer than 10 characters not present."

	print "Word longer than 10 characters present on page."
	print "Longest word is", (max(long_words, key=lambda s: (len(s), s)))

def image_test(driver, image_source):
	#Image dictionary
	image_dict = {"https://the-internet.herokuapp.com/img/avatars/Original-Facebook-Geek-Profile-Avatar-1.jpg": "Mario", "https://the-internet.herokuapp.com/img/avatars/Original-Facebook-Geek-Profile-Avatar-6.jpg": "Storm Trooper", "https://the-internet.herokuapp.com/img/avatars/Original-Facebook-Geek-Profile-Avatar-3.jpg": "Punisher", "https://the-internet.herokuapp.com/img/avatars/Original-Facebook-Geek-Profile-Avatar-2.jpg": "Bobba Fet", "https://the-internet.herokuapp.com/img/avatars/Original-Facebook-Geek-Profile-Avatar-7.jpg": "Harley Quinn"}

	images_found = []
	punisher_found = False

	try:
                elements = (driver.find_elements_by_tag_name("img"))
        except NoSuchElementException as error:
                print error
                sys.exit()

	for element in elements:
		if (element.get_attribute('src') == punisher_src):
			punisher_found = True
		if (element.get_attribute('src') in image_dict.keys()):
			images_found.append(image_dict[element.get_attribute('src')])

        onscreen = ",".join(images_found)
        print onscreen, "found on screen."

	try:
                assert (not punisher_found)
        except AssertionError:
                print "Punisher image is present on the page."
		print "Sleeping for 60 seconds ..."
		time.sleep(60)
        	driver.close()
                sys.exit()

	print "Punisher image not present on the page."

if __name__ == '__main__':
	driver = webdriver.Firefox()
	url = "https://the-internet.herokuapp.com/dynamic_content"
	word_path = '//div[@class="large-10 columns"]'
	word_test(driver, url, word_path)
	image_path = '//div[@class="large-2 columns"]'
	punisher_src = "https://the-internet.herokuapp.com/img/avatars/Original-Facebook-Geek-Profile-Avatar-3.jpg"
	image_test(driver, punisher_src)
	print "Sleeping for 60 seconds ..."
	time.sleep(60)
	driver.close()
