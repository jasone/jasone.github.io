targets:
	@echo "Available targets: setup update serve clean"

setup:
	bundle config set --local path 'vendor/bundle'
	bundle install

update:
	bundle update

serve:
	rm -rf _site
	bundle exec jekyll serve --incremental --drafts

clean:
	rm -rf .bundle
	rm -f .jekyll-metadata
	rm -f Gemfile.lock
	rm -rf _site
	rm -rf vendor
